#include <muduo/base/Atomic.h>
#include <muduo/base/Logging.h>
#include <muduo/base/Mutex.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/EventLoopThreadPool.h>
#include <muduo/net/TcpClient.h>

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <assert.h>
#include <stdio.h>

using namespace muduo;
using namespace muduo::net;

AtomicInt32 g_aliveConnections;
AtomicInt32 g_disaliveConnections;
int g_connections = 4;
EventLoop* g_loop;

class RecvFileClient : boost::noncopyable
{
 public:
  RecvFileClient(EventLoop* loop, const InetAddress& serverAddr, const string& id)
    : loop_(loop),
      client_(loop, serverAddr, "RecvFileClient")
  {
    client_.setConnectionCallback(
        boost::bind(&RecvFileClient::onConnection, this, _1));
    client_.setMessageCallback(
        boost::bind(&RecvFileClient::onMessage, this, _1, _2, _3));
    string filename = "RecvFileClient" + id;
    fp_ = ::fopen(filename.c_str(), "we");
    assert(fp_);
  }

  ~RecvFileClient()
  {
    ::fclose(fp_);
  }

  void connect()
  {
    client_.connect();
  }

 private:
  void onConnection(const TcpConnectionPtr& conn)
  {
    LOG_INFO << conn->localAddress().toIpPort() << " -> "
             << conn->peerAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN");

    if (conn->connected())
    {
      connection_ = conn;
      if (g_aliveConnections.incrementAndGet() == g_connections)
        LOG_INFO << "all connected";
    }
    else
    {
      connection_.reset();
	  
      if (g_disaliveConnections.incrementAndGet() == g_connections)
      {
        LOG_INFO << "all disconnected";
        g_loop->quit();
        //exit(0);
      }
    }
  }

  void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time)
  {
    fwrite(buf->peek(), 1, buf->readableBytes(), fp_);
	buf->retrieveAll();
  }

  EventLoop* loop_;
  TcpClient client_;
  TcpConnectionPtr connection_;
  FILE* fp_;
};

int main(int argc, char* argv[])
{
  LOG_INFO << "pid = " << getpid();
  EventLoop loop;
  g_loop = &loop;
  // 用两个IO线程来发起大量的连接
  EventLoopThreadPool loopPool(&loop);
  loopPool.setThreadNum(2);
  loopPool.start();

  boost::ptr_vector<RecvFileClient> clients(g_connections);

  InetAddress serverAddr("127.0.0.1", 2021);

  for (int i = 0; i < g_connections; ++i)
  {
    char buf[32];
    snprintf(buf, sizeof buf, "%d", i+1);
    clients.push_back(new RecvFileClient(loopPool.getNextLoop(), serverAddr, buf));
    clients[i].connect();
    usleep(200);
  }

  loop.loop();
  usleep(20000);
}


