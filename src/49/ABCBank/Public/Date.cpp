#include "Date.h"
#include <assert.h>

using namespace PUBLIC;

Date::Date(int y, int m, int d)
{
	assert( y >= 1900 );
	assert( m >= 1 && m <= 12 );
	assert( d >= 1 && d <= 31 );

	struct tm atm;

	atm.tm_sec = 0;
	atm.tm_min = 0;
	atm.tm_hour = 0;
	atm.tm_mday = d;
	atm.tm_mon = m - 1;        // tm_mon is 0 based
	atm.tm_year = y - 1900;     // tm_year is 1900 based
	atm.tm_isdst = -1;

	time_ = _mktime64(&atm);
}

std::string Date::toIsoString() const
{
	char buf[32] = {0};
	_snprintf_s(buf, sizeof buf, _TRUNCATE, "%4d-%02d-%02d", year(), month(), day());
	return buf;
}

int Date::year() const
{
	struct tm ttm;
	struct tm * ptm;

	ptm = getLocalTm(&ttm);
	return ptm ? (ptm->tm_year) + 1900 : 0 ;
}

int Date::month() const
{
	struct tm ttm;
	struct tm * ptm;

	ptm = getLocalTm(&ttm);
	return ptm ? ptm->tm_mon + 1 : 0;
}

int Date::day() const
{
	struct tm ttm;
	struct tm * ptm;

	ptm = getLocalTm(&ttm);
	return ptm ? ptm->tm_mday : 0 ; 
}

// [0, 1, ..., 6] => [Sunday, Monday, ..., Saturday ]
int Date::weekDay() const
{
	struct tm ttm;
	struct tm * ptm;

	ptm = getLocalTm(&ttm);
	return ptm ? ptm->tm_wday + 1 : 0 ;
}

struct tm* Date::getLocalTm(struct tm* ptm) const
{
	if (ptm != NULL)
	{
		struct tm ptmTemp;
		errno_t err = _localtime64_s(&ptmTemp, &time_);

		if (err != 0)
		{
			return NULL;    // indicates that m_time was not initialized!
		}

		*ptm = ptmTemp;
		return ptm;
	}

	return NULL;
}