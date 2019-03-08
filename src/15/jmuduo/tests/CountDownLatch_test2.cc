#include <muduo/base/CountDownLatch.h>
#include <muduo/base/Thread.h>

#include <boost/bind.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <string>
#include <stdio.h>

using namespace muduo;

class Test
{
 public:
  Test(int numThreads)
    : latch_(numThreads),
      threads_(numThreads)
  {
    for (int i = 0; i < numThreads; ++i)
    {
      char name[32];
      snprintf(name, sizeof name, "work thread %d", i);
      threads_.push_back(new muduo::Thread(
            boost::bind(&Test::threadFunc, this), muduo::string(name)));
    }
    for_each(threads_.begin(), threads_.end(), boost::bind(&muduo::Thread::start, _1));
  }

  void wait()
  {
    latch_.wait();
  }

  void joinAll()
  {
    for_each(threads_.begin(), threads_.end(), boost::bind(&Thread::join, _1));
  }

 private:

  void threadFunc()
  {
    sleep(3);
    latch_.countDown();
    printf("tid=%d, %s started\n",
           CurrentThread::tid(),
           CurrentThread::name());

    

    printf("tid=%d, %s stopped\n",
           CurrentThread::tid(),
           CurrentThread::name());
  }

  CountDownLatch latch_;
  boost::ptr_vector<Thread> threads_;
};

int main()
{
  printf("pid=%d, tid=%d\n", ::getpid(), CurrentThread::tid());
  Test t(3);
  t.wait();
  printf("pid=%d, tid=%d %s running ...\n", ::getpid(), CurrentThread::tid(), CurrentThread::name());
  t.joinAll();

  printf("number of created threads %d\n", Thread::numCreated());
}


