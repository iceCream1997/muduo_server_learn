// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//

#include <muduo/net/inspect/Inspector.h>

#include <muduo/base/Thread.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/http/HttpRequest.h>
#include <muduo/net/http/HttpResponse.h>
#include <muduo/net/inspect/ProcessInspector.h>

//#include <iostream>
//#include <iterator>
//#include <sstream>
#include <boost/bind.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

using namespace muduo;
using namespace muduo::net;

namespace
{
Inspector* g_globalInspector = 0;

// Looks buggy
std::vector<string> split(const string& str)
{
  std::vector<string> result;
  size_t start = 0;
  size_t pos = str.find('/');
  while (pos != string::npos)
  {
    if (pos > start)
    {
      result.push_back(str.substr(start, pos-start));
    }
    start = pos+1;
    pos = str.find('/', start);
  }

  if (start < str.length())		// 说明最后一个字符不是'/'
  {
    result.push_back(str.substr(start));
  }

  return result;
}

}

Inspector::Inspector(EventLoop* loop,
                     const InetAddress& httpAddr,
                     const string& name)
    : server_(loop, httpAddr, "Inspector:"+name),
      processInspector_(new ProcessInspector)
{
  assert(CurrentThread::isMainThread());
  assert(g_globalInspector == 0);
  g_globalInspector = this;
  server_.setHttpCallback(boost::bind(&Inspector::onRequest, this, _1, _2));
  processInspector_->registerCommands(this);
  // 这样子做法是为了防止竞态问题
  // 如果直接调用start，（当前线程不是loop所属的IO线程，是主线程）那么有可能，当前构造函数还没返回，
  // HttpServer所在的IO线程可能已经收到了http客户端的请求了（因为这时候HttpServer已启动），那么就会回调
  // Inspector::onRequest，而这时候构造函数还没返回，也就是说对象还没完全构造好
  loop->runAfter(0, boost::bind(&Inspector::start, this)); // little race condition
}

Inspector::~Inspector()
{
  assert(CurrentThread::isMainThread());
  g_globalInspector = NULL;
}

void Inspector::add(const string& module,
                    const string& command,
                    const Callback& cb,
                    const string& help)
{
  MutexLockGuard lock(mutex_);
  commands_[module][command] = cb;
  helps_[module][command] = help;
}

void Inspector::start()
{
  server_.start();
}

void Inspector::onRequest(const HttpRequest& req, HttpResponse* resp)
{
  if (req.path() == "/")
  {
    string result;
    MutexLockGuard lock(mutex_);
    // 遍历helps 
    for (std::map<string, HelpList>::const_iterator helpListI = helps_.begin();
         helpListI != helps_.end();
         ++helpListI)
    {
      const HelpList& list = helpListI->second;
      for (HelpList::const_iterator it = list.begin();
           it != list.end();
           ++it)
      {
        result += "/";
        result += helpListI->first;		// module
        result += "/";
        result += it->first;			// command
        result += "\t";
        result += it->second;			// help
        result += "\n";
      }
    }
    resp->setStatusCode(HttpResponse::k200Ok);
    resp->setStatusMessage("OK");
    resp->setContentType("text/plain");
    resp->setBody(result);
  }
  else
  {
    // 以"/"进行分割，将得到的字符串保存在result中
    std::vector<string> result = split(req.path());
    // boost::split(result, req.path(), boost::is_any_of("/"));
    //std::copy(result.begin(), result.end(), std::ostream_iterator<string>(std::cout, ", "));
    //std::cout << "\n";
    bool ok = false;
    if (result.size() == 0)
    {
      // 这种情况是错误的，因此ok仍为false
    }
    else if (result.size() == 1)
    {
      // 只有module，没有command也是错的，因此ok仍为false
      string module = result[0];
    }
    else
    {
      string module = result[0];
      // 查找module所对应的命令列表
      std::map<string, CommandList>::const_iterator commListI = commands_.find(module);
      if (commListI != commands_.end())
      {
        string command = result[1];
        const CommandList& commList = commListI->second;
		// 查找command对应的命令
        CommandList::const_iterator it = commList.find(command);
        if (it != commList.end())
        {
          ArgList args(result.begin()+2, result.end());		// 传递给回调函数的参数表
          if (it->second)
          {
            resp->setStatusCode(HttpResponse::k200Ok);
            resp->setStatusMessage("OK");
            resp->setContentType("text/plain");
            const Callback& cb = it->second;
            resp->setBody(cb(req.method(), args));		// 调用cb将返回的字符串传给setBody
            ok = true;
          }
        }
      }

    }

    if (!ok)
    {
      resp->setStatusCode(HttpResponse::k404NotFound);
      resp->setStatusMessage("Not Found");
    }
    //resp->setCloseConnection(true);
  }
}

