#ifndef _STRING_UTIL_H_
#define _STRING_UTIL_H_

#include <vector>
#include <string>

namespace PUBLIC
{

class StringUtil  
{
public:
	static std::string Trim(std::string& str);
	static std::string Ltrim(std::string& str);
	static std::string Rtrim(std::string& str);
	static std::vector<std::string> Split(const std::string& str, const char delim);
};

}

#endif // _STRING_UTIL_H_
