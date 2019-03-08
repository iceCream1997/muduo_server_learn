#include "JUtil.h"

#include <iomanip>
#include <sstream>
using namespace PUBLIC;
using namespace std;

int Convert::StringToInt(const string& str)
{
	stringstream ss;
	ss<<str;
	int x;
	ss>>x;

	return x;
}

string Convert::IntToString(int x)
{
	stringstream ss;
	ss<<x;
	string str;
	ss>>str;

	return str;
}

double Convert::StringToDouble(const string& str)
{
	stringstream ss;
	ss<<str;
	double x;
	ss>>x;

	return x;
}

string Convert::DoubleToString(double x)
{
	stringstream ss;
	ss<<setprecision(2)<<setiosflags(ios::fixed)<<x;
	string str;
	ss>>str;

	return str;
}