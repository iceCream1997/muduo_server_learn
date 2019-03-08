#include <muduo/base/Logging.h>
#include <errno.h>
#include <stdio.h>

using namespace muduo;

FILE* g_file;

void dummyOutput(const char* msg, int len)
{
	if (g_file)
	{
		fwrite(msg, 1, len, g_file);
	}
}

void dummyFlush()
{
	fflush(g_file);
}

int main()
{
	g_file = ::fopen("/tmp/muduo_log", "ae");
	Logger::setOutput(dummyOutput);
	Logger::setFlush(dummyFlush);

	LOG_TRACE<<"trace ...";
	LOG_DEBUG<<"debug ...";
	LOG_INFO<<"info ...";
	LOG_WARN<<"warn ...";
	LOG_ERROR<<"error ...";
	//LOG_FATAL<<"fatal ...";
	errno = 13;
	LOG_SYSERR<<"syserr ...";
	//LOG_SYSFATAL<<"sysfatal ...";

	::fclose(g_file);

	return 0;
}
