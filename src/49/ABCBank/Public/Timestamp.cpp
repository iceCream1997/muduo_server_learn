#include "Timestamp.h"

using namespace PUBLIC;


Timestamp::Timestamp(__time64_t time) : time_(time)
{
}

std::string Timestamp::toFormattedString() const
{
	char buf[32] = {0};
	struct tm tm_time;

	_gmtime64_s(&tm_time, &time_);


	_snprintf_s(buf, sizeof(buf), _TRUNCATE, "%4d-%02d-%02d %02d:%02d:%02d",
		tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
		tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
	return buf;
}

Timestamp Timestamp::now()
{
	return Timestamp(::_time64(NULL));
}

Timestamp Timestamp::invalid()
{
	return Timestamp();
}

