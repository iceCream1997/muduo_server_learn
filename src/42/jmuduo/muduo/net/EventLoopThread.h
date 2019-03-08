// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is a public header file, it must only include public header files.

#ifndef MUDUO_NET_EVENTLOOPTHREAD_H
#define MUDUO_NET_EVENTLOOPTHREAD_H

#include <muduo/base/Condition.h>
#include <muduo/base/Mutex.h>
#include <muduo/base/Thread.h>

#include <boost/noncopyable.hpp>

namespace muduo
{
namespace net
{

class EventLoop;

class EventLoopThread : boost::noncopyable
{
 public:
  typedef boost::function<void(EventLoop*)> ThreadInitCallback;

  EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback());
  ~EventLoopThread();
  EventLoop* startLoop();	// 启动线程，该线程就成为了IO线程

 private:
  void threadFunc();		// 线程函数

  EventLoop* loop_;			// loop_指针指向一个EventLoop对象
  bool exiting_;
  Thread thread_;
  MutexLock mutex_;
  Condition cond_;
  ThreadInitCallback callback_;		// 回调函数在EventLoop::loop事件循环之前被调用
};

}
}

#endif  // MUDUO_NET_EVENTLOOPTHREAD_H

