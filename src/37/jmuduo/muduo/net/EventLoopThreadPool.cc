// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include <muduo/net/EventLoopThreadPool.h>

#include <muduo/net/EventLoop.h>
#include <muduo/net/EventLoopThread.h>

#include <boost/bind.hpp>

using namespace muduo;
using namespace muduo::net;


EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop)
  : baseLoop_(baseLoop),
    started_(false),
    numThreads_(0),
    next_(0)
{
}

EventLoopThreadPool::~EventLoopThreadPool()
{
  // Don't delete loop, it's stack variable
}

void EventLoopThreadPool::start(const ThreadInitCallback& cb)
{
  assert(!started_);
  baseLoop_->assertInLoopThread();

  started_ = true;

  for (int i = 0; i < numThreads_; ++i)
  {
    EventLoopThread* t = new EventLoopThread(cb);
    threads_.push_back(t);
    loops_.push_back(t->startLoop());	// 启动EventLoopThread线程，在进入事件循环之前，会调用cb
  }
  if (numThreads_ == 0 && cb)
  {
    // 只有一个EventLoop，在这个EventLoop进入事件循环之前，调用cb
    cb(baseLoop_);
  }
}

EventLoop* EventLoopThreadPool::getNextLoop()
{
  baseLoop_->assertInLoopThread();
  EventLoop* loop = baseLoop_;

  // 如果loops_为空，则loop指向baseLoop_
  // 如果不为空，按照round-robin（RR，轮叫）的调度方式选择一个EventLoop
  if (!loops_.empty())
  {
    // round-robin
    loop = loops_[next_];
    ++next_;
    if (implicit_cast<size_t>(next_) >= loops_.size())
    {
      next_ = 0;
    }
  }
  return loop;
}

