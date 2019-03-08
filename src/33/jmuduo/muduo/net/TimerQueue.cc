// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#define __STDC_LIMIT_MACROS
#include <muduo/net/TimerQueue.h>

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/Timer.h>
#include <muduo/net/TimerId.h>

#include <boost/bind.hpp>

#include <sys/timerfd.h>

namespace muduo
{
namespace net
{
namespace detail
{

// 创建定时器
int createTimerfd()
{
  int timerfd = ::timerfd_create(CLOCK_MONOTONIC,
                                 TFD_NONBLOCK | TFD_CLOEXEC);
  if (timerfd < 0)
  {
    LOG_SYSFATAL << "Failed in timerfd_create";
  }
  return timerfd;
}

// 计算超时时刻与当前时间的时间差
struct timespec howMuchTimeFromNow(Timestamp when)
{
  int64_t microseconds = when.microSecondsSinceEpoch()
                         - Timestamp::now().microSecondsSinceEpoch();
  if (microseconds < 100)
  {
    microseconds = 100;
  }
  struct timespec ts;
  ts.tv_sec = static_cast<time_t>(
      microseconds / Timestamp::kMicroSecondsPerSecond);
  ts.tv_nsec = static_cast<long>(
      (microseconds % Timestamp::kMicroSecondsPerSecond) * 1000);
  return ts;
}

// 清除定时器，避免一直触发
void readTimerfd(int timerfd, Timestamp now)
{
  uint64_t howmany;
  ssize_t n = ::read(timerfd, &howmany, sizeof howmany);
  LOG_TRACE << "TimerQueue::handleRead() " << howmany << " at " << now.toString();
  if (n != sizeof howmany)
  {
    LOG_ERROR << "TimerQueue::handleRead() reads " << n << " bytes instead of 8";
  }
}

// 重置定时器的超时时间
void resetTimerfd(int timerfd, Timestamp expiration)
{
  // wake up loop by timerfd_settime()
  struct itimerspec newValue;
  struct itimerspec oldValue;
  bzero(&newValue, sizeof newValue);
  bzero(&oldValue, sizeof oldValue);
  newValue.it_value = howMuchTimeFromNow(expiration);
  int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
  if (ret)
  {
    LOG_SYSERR << "timerfd_settime()";
  }
}

}
}
}

using namespace muduo;
using namespace muduo::net;
using namespace muduo::net::detail;

TimerQueue::TimerQueue(EventLoop* loop)
  : loop_(loop),
    timerfd_(createTimerfd()),
    timerfdChannel_(loop, timerfd_),
    timers_(),
    callingExpiredTimers_(false)
{
  timerfdChannel_.setReadCallback(
      boost::bind(&TimerQueue::handleRead, this));
  // we are always reading the timerfd, we disarm it with timerfd_settime.
  timerfdChannel_.enableReading();
}

TimerQueue::~TimerQueue()
{
  ::close(timerfd_);
  // do not remove channel, since we're in EventLoop::dtor();
  for (TimerList::iterator it = timers_.begin();
      it != timers_.end(); ++it)
  {
    delete it->second;
  }
}

TimerId TimerQueue::addTimer(const TimerCallback& cb,
                             Timestamp when,
                             double interval)
{
  Timer* timer = new Timer(cb, when, interval);

  loop_->runInLoop(
      boost::bind(&TimerQueue::addTimerInLoop, this, timer));
	  
  //addTimerInLoop(timer);
  return TimerId(timer, timer->sequence());
}

void TimerQueue::cancel(TimerId timerId)
{
  loop_->runInLoop(
      boost::bind(&TimerQueue::cancelInLoop, this, timerId));
  //cancelInLoop(timerId);
}

void TimerQueue::addTimerInLoop(Timer* timer)
{
  loop_->assertInLoopThread();
  // 插入一个定时器，有可能会使得最早到期的定时器发生改变
  bool earliestChanged = insert(timer);

  if (earliestChanged)
  {
    // 重置定时器的超时时刻(timerfd_settime)
    resetTimerfd(timerfd_, timer->expiration());
  }
}

void TimerQueue::cancelInLoop(TimerId timerId)
{
  loop_->assertInLoopThread();
  assert(timers_.size() == activeTimers_.size());
  ActiveTimer timer(timerId.timer_, timerId.sequence_);
  // 查找该定时器
  ActiveTimerSet::iterator it = activeTimers_.find(timer);
  if (it != activeTimers_.end())
  {
    size_t n = timers_.erase(Entry(it->first->expiration(), it->first));
    assert(n == 1); (void)n;
    delete it->first; // FIXME: no delete please,如果用了unique_ptr,这里就不需要手动删除了
    activeTimers_.erase(it);
  }
  else if (callingExpiredTimers_)
  {
    // 已经到期，并且正在调用回调函数的定时器
    cancelingTimers_.insert(timer);
  }
  assert(timers_.size() == activeTimers_.size());
}

void TimerQueue::handleRead()
{
  loop_->assertInLoopThread();
  Timestamp now(Timestamp::now());
  readTimerfd(timerfd_, now);		// 清除该事件，避免一直触发

  // 获取该时刻之前所有的定时器列表(即超时定时器列表)
  std::vector<Entry> expired = getExpired(now);

  callingExpiredTimers_ = true;
  cancelingTimers_.clear();
  // safe to callback outside critical section
  for (std::vector<Entry>::iterator it = expired.begin();
      it != expired.end(); ++it)
  {
    // 这里回调定时器处理函数
    it->second->run();
  }
  callingExpiredTimers_ = false;

  // 不是一次性定时器，需要重启
  reset(expired, now);
}

// rvo
std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now)
{
  assert(timers_.size() == activeTimers_.size());
  std::vector<Entry> expired;
  Entry sentry(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
  // 返回第一个未到期的Timer的迭代器
  // lower_bound的含义是返回第一个值>=sentry的元素的iterator
  // 即*end >= sentry，从而end->first > now
  TimerList::iterator end = timers_.lower_bound(sentry);
  assert(end == timers_.end() || now < end->first);
  // 将到期的定时器插入到expired中
  std::copy(timers_.begin(), end, back_inserter(expired));
  // 从timers_中移除到期的定时器
  timers_.erase(timers_.begin(), end);

  // 从activeTimers_中移除到期的定时器
  for (std::vector<Entry>::iterator it = expired.begin();
      it != expired.end(); ++it)
  {
    ActiveTimer timer(it->second, it->second->sequence());
    size_t n = activeTimers_.erase(timer);
    assert(n == 1); (void)n;
  }

  assert(timers_.size() == activeTimers_.size());
  return expired;
}

void TimerQueue::reset(const std::vector<Entry>& expired, Timestamp now)
{
  Timestamp nextExpire;

  for (std::vector<Entry>::const_iterator it = expired.begin();
      it != expired.end(); ++it)
  {
    ActiveTimer timer(it->second, it->second->sequence());
    // 如果是重复的定时器并且是未取消定时器，则重启该定时器
    if (it->second->repeat()
        && cancelingTimers_.find(timer) == cancelingTimers_.end())
    {
      it->second->restart(now);
      insert(it->second);
    }
    else
    {
      // 一次性定时器或者已被取消的定时器是不能重置的，因此删除该定时器
      // FIXME move to a free list
      delete it->second; // FIXME: no delete please
    }
  }

  if (!timers_.empty())
  {
    // 获取最早到期的定时器超时时间
    nextExpire = timers_.begin()->second->expiration();
  }

  if (nextExpire.valid())
  {
    // 重置定时器的超时时刻(timerfd_settime)
    resetTimerfd(timerfd_, nextExpire);
  }
}

bool TimerQueue::insert(Timer* timer)
{
  loop_->assertInLoopThread();
  assert(timers_.size() == activeTimers_.size());
  // 最早到期时间是否改变
  bool earliestChanged = false;
  Timestamp when = timer->expiration();
  TimerList::iterator it = timers_.begin();
  // 如果timers_为空或者when小于timers_中的最早到期时间
  if (it == timers_.end() || when < it->first)
  {
    earliestChanged = true;
  }
  {
    // 插入到timers_中
    std::pair<TimerList::iterator, bool> result
      = timers_.insert(Entry(when, timer));
    assert(result.second); (void)result;
  }
  {
    // 插入到activeTimers_中
    std::pair<ActiveTimerSet::iterator, bool> result
      = activeTimers_.insert(ActiveTimer(timer, timer->sequence()));
    assert(result.second); (void)result;
  }

  assert(timers_.size() == activeTimers_.size());
  return earliestChanged;
}

