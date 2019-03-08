#include "Validator.h"

namespace UI
{

// 用户名长度<10位，字母
bool ValidateName(const std::string& str, char ch)
{
	return str.length() < 10 && isalnum(ch);
}

// 密码6-8位
bool ValidatePass(const std::string& str, char ch)
{
	// 密码可以是任意可打印字符
	return str.length() < 8;
}

// 身份证18位
bool ValidateId(const std::string& str, char ch)
{
	if (str.length() >= 18)
		return false;

	if (!isdigit(ch) && ch != 'x' && ch != 'X')
		return false;

	if ((ch == 'x' || ch == 'X') && str.length() != 17)	// 只有最后一位可以是x或者X
		return false;

	// TODO身份证中日期有效性验证

	return true;
}

bool ValidateAccountId(const std::string& str, char ch)
{
	return str.length() < 6 && isdigit(ch);
}

// decimal(10,2),整数部分不超过8位，小数部分不超过2位，总长度不超过11位（包括小数点）
bool ValidateMoney(const std::string& str, char ch)
{
	if (str.length() >= 11 || (!isdigit(ch) && ch != '.'))
		return false;


	bool findDot = false;
	size_t dotPos = std::string::npos;
	dotPos = str.find('.');
	if (dotPos != std::string::npos)
		findDot = true;


	if (findDot)
	{
		if (ch == '.')
			return false;	// 不能出现两个点号
		else if (str.length()-1>=dotPos+2)	// 小数点后最多2位
			return false;
	}
	else if (ch != '.' && str.length() >= 8)	// 小数点前面最多8位
		return false;

	return true;
}

bool ValidateDate(const std::string& str, char ch)
{
	// 日期不超过10位，只能是数字或者‘-’
	// TODO日期有效性验证
	return str.length() < 10 && (isdigit(ch) || ch == '-');
}

}
