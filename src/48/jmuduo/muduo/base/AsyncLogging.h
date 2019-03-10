#ifndef MUDUO_BASE_ASYNCLOGGING_H
#define MUDUO_BASE_ASYNCLOGGING_H

#include <muduo/base/BlockingQueue.h>
#include <muduo/base/BoundedBlockingQueue.h>
#include <muduo/base/CountDownLatch.h>
#include <muduo/base/Mutex.h>
#include <muduo/base/Thread.h>

#include <muduo/base/LogStream.h>

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace muduo
{

class AsyncLogging : boost::noncopyable
{
 public:

  AsyncLogging(const string& basename,
               size_t rollSize,
               int flushInterval = 3);

  ~AsyncLogging()
  {
    if (running_)
    {
      stop();
    }
  }

  // 供前端生产者线程调用（日志数据写到缓冲区）
  void append(const char* logline, int len);

  void start()
  {
    running_ = true;
    thread_.start(); // 日志线程启动
    latch_.wait();
  }

  void stop()
  {
    running_ = false;
    cond_.notify();
    thread_.join();
  }

 private:

  // declare but not define, prevent compiler-synthesized functions
  AsyncLogging(const AsyncLogging&);  // ptr_container
  void operator=(const AsyncLogging&);  // ptr_container

  // 供后端消费者线程调用（将数据写到日志文件）
  void threadFunc();

  typedef muduo::detail::FixedBuffer<muduo::detail::kLargeBuffer> Buffer;
  typedef boost::ptr_vector<Buffer> BufferVector;
  typedef BufferVector::auto_type BufferPtr; // 可理解为Buffer的智能指针，能管理Buffer的生存期，
                                             // 类似于C++11中的unique_ptr，具备移动语义
                                             // （两个unique_ptr不能指向一个对象，不能进行复制操作只能进行移动操作）

  const int flushInterval_; // 超时时间，在flushInterval_秒内，缓冲区没写满，仍将缓冲区中的数据写到文件中
  bool running_;
  string basename_;
  size_t rollSize_;
  muduo::Thread thread_;
  muduo::CountDownLatch latch_;  // 用于等待线程启动
  muduo::MutexLock mutex_;
  muduo::Condition cond_;
  BufferPtr currentBuffer_; // 当前缓冲区
  BufferPtr nextBuffer_;    // 预备缓冲区
  BufferVector buffers_;    // 待写入文件的已填满的缓冲区
};

}
#endif  // MUDUO_BASE_ASYNCLOGGING_H
