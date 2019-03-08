#ifndef _VALIDATOR_H_
#define _VALIDATOR_H_

#include <ctype.h>
#include <string>

namespace UI
{

bool ValidateName(const std::string& str, char ch);
bool ValidatePass(const std::string& str, char ch);
bool ValidateId(const std::string& str, char ch);
bool ValidateAccountId(const std::string& str, char ch);
bool ValidateMoney(const std::string& str, char ch);
bool ValidateDate(const std::string& str, char ch);

}
#endif // _VALIDATOR_H_