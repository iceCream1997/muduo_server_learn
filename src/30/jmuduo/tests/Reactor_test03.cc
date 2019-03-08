#include <muduo/net/Channel.h>
#include <muduo/net/EventLoop.h>

#include <boost/bind.hpp>

#include <stdio.h>
#include <sys/timerfd.h>

using namespace muduo;
using namespace muduo::net;

EventLoop* g_loop;
int timerfd;

void timeout(Timestamp receiveTime)
{
	printf("Timeout!\n");
	uint64_t howmany;
	::read(timerfd, &howmany, sizeof howmany);
	g_loop->quit();
}

int main(void)
{
	EventLoop loop;
	g_loop = &loop;

	timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
	Channel channel(&loop, timerfd);
	channel.setReadCallback(boost::bind(timeout, _1));
	channel.enableReading();

	struct itimerspec howlong;
	bzero(&howlong, sizeof howlong);
	howlong.it_value.tv_sec = 1;
	::timerfd_settime(timerfd, 0, &howlong, NULL);

	loop.loop();

	::close(timerfd);
}



