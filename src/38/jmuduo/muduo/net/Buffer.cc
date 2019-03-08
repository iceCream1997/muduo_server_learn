// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//

#include <muduo/net/Buffer.h>

#include <muduo/net/SocketsOps.h>

#include <errno.h>
#include <sys/uio.h>

using namespace muduo;
using namespace muduo::net;

const char Buffer::kCRLF[] = "\r\n";

const size_t Buffer::kCheapPrepend;
const size_t Buffer::kInitialSize;

// 结合栈上的空间，避免内存使用过大，提高内存使用率
// 如果有5K个连接，每个连接就分配64K+64K的缓冲区的话，将占用640M内存，
// 而大多数时候，这些缓冲区的使用率很低
ssize_t Buffer::readFd(int fd, int* savedErrno)
{
  // saved an ioctl()/FIONREAD call to tell how much to read
  // 节省一次ioctl系统调用（获取有多少可读数据）
  char extrabuf[65536];
  struct iovec vec[2];
  const size_t writable = writableBytes();
  // 第一块缓冲区
  vec[0].iov_base = begin()+writerIndex_;
  vec[0].iov_len = writable;
  // 第二块缓冲区
  vec[1].iov_base = extrabuf;
  vec[1].iov_len = sizeof extrabuf;
  const ssize_t n = sockets::readv(fd, vec, 2);
  if (n < 0)
  {
    *savedErrno = errno;
  }
  else if (implicit_cast<size_t>(n) <= writable)	//第一块缓冲区足够容纳
  {
    writerIndex_ += n;
  }
  else		// 当前缓冲区，不够容纳，因而数据被接收到了第二块缓冲区extrabuf，将其append至buffer
  {
    writerIndex_ = buffer_.size();
    append(extrabuf, n - writable);
  }
  // if (n == writable + sizeof extrabuf)
  // {
  //   goto line_30;
  // }
  return n;
}

