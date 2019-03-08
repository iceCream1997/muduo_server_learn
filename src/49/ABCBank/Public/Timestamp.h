#ifndef _TIMESTAMP_H_
#define _TIMESTAMP_H_

#include <time.h>
#include <stdio.h>
#include <string>

namespace PUBLIC
{

class Timestamp
{
public:
	Timestamp() : time_(0)
	{
	}

	explicit Timestamp(__time64_t time);

	void swap(Timestamp& that)
	{
		std::swap(time_, that.time_);
	}

	std::string toString() const;
	std::string toFormattedString() const;

	bool valid() const { return time_ > 0; }

	__time64_t getTime() const { return time_; }

	static Timestamp now();
	static Timestamp invalid();

private:
	__time64_t time_;
};

inline bool operator<(Timestamp lhs, Timestamp rhs)
{
	return lhs.getTime() < rhs.getTime();
}

inline bool operator==(Timestamp lhs, Timestamp rhs)
{
	return lhs.getTime() == rhs.getTime();
}

inline __time64_t timeDifference(Timestamp high, Timestamp low)
{
	__time64_t diff = high.getTime() - low.getTime();
	return diff;
}

inline Timestamp addTime(Timestamp timestamp, __time64_t seconds)
{
	return Timestamp(timestamp.getTime() + seconds);
}

}

#endif  // _TIMESTAMP_H_
