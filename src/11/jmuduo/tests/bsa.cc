#include <boost/static_assert.hpp>

class Timestamp
{
private:
	int64_t microSecondsSinceEpoch_;
};

BOOST_STATIC_ASSERT(sizeof(Timestamp) == sizeof(int64_t));
//BOOST_STATIC_ASSERT(sizeof(int) == sizeof(short));

int main(void)
{
	return 0;
}
