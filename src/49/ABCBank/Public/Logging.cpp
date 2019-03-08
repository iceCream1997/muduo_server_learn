#include "Logging.h"
#include "Timestamp.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <sstream>

namespace PUBLIC
{

Logger::LogLevel initLogLevel()
{
	return Logger::INFO;
}

Logger::LogLevel g_logLevel = initLogLevel();

const char* LogLevelName[Logger::NUM_LOG_LEVELS] =
{
	"TRACE ",
	"DEBUG ",
	"INFO  ",
	"WARN  ",
	"ERROR ",
	"FATAL ",
};

class T
{
public:
	T(const char* str, unsigned len)
		:str_(str),
		len_(len)
	{
		assert(strlen(str) == len_);
	}

	const char* str_;
	const unsigned len_;
};

inline LogStream& operator<<(LogStream& s, T v)
{
	s.append(v.str_, v.len_);
	return s;
}

inline LogStream& operator<<(LogStream& s, const Logger::SourceFile& v)
{
	s.append(v.data_, v.size_);
	return s;
}

void defaultOutput(const char* msg, int len)
{
	size_t n = fwrite(msg, 1, len, stdout);
	(void)n;
}

void defaultFlush()
{
	fflush(stdout);
}

Logger::OutputFunc g_output = defaultOutput;
Logger::FlushFunc g_flush = defaultFlush;

Logger::Impl::Impl(LogLevel level, int savedErrno, const SourceFile& file, int line)
	: time_(Timestamp::now()),
	  stream_(),
	  level_(level),
	  line_(line),
	  basename_(file)
{
	formatTime();
	stream_ << T(LogLevelName[level], 6);
	if (savedErrno != 0)
	{
		char buf[100] = {0};
		strerror_s(buf, 100, savedErrno);
		stream_ << T(buf, strlen(buf)) << " (errno=" << savedErrno << ") ";
	}
}

void Logger::Impl::formatTime()
{
	__time64_t seconds = time_.getTime();

	struct tm tm_time;
	_gmtime64_s(&tm_time, &seconds);

	char buf[32] = {0};
	int len = _snprintf_s(buf, sizeof(buf), _TRUNCATE, "%4d-%02d-%02d %02d:%02d:%02d ",
		tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
		tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);

	stream_ << T(buf, len);
}

void Logger::Impl::finish()
{
	stream_ << " - " << basename_ << ':' << line_ << '\n';
}

Logger::Logger(SourceFile file, int line)
	: impl_(INFO, 0, file, line)
{
}

Logger::Logger(SourceFile file, int line, LogLevel level, const char* func)
	: impl_(level, 0, file, line)
{
	impl_.stream_ << func << ' ';
}

Logger::Logger(SourceFile file, int line, LogLevel level)
	: impl_(level, 0, file, line)
{
}

Logger::Logger(SourceFile file, int line, bool toAbort)
	: impl_(toAbort?FATAL:ERROR, errno, file, line)
{
}

Logger::~Logger()
{
	impl_.finish();
	const LogStream::Buffer& buf(stream().buffer());
	g_output(buf.data(), buf.length());
	if (impl_.level_ == FATAL)
	{
		g_flush();
		abort();
	}
}

void Logger::setLogLevel(Logger::LogLevel level)
{
	g_logLevel = level;
}

void Logger::setOutput(OutputFunc out)
{
	g_output = out;
}

void Logger::setFlush(FlushFunc flush)
{
	g_flush = flush;
}

}
