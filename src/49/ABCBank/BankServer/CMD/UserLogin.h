#ifndef _USER_LOGIN_H_
#define _USER_LOGIN_H_

#include "Transaction.h"

namespace CMD
{

// ¹ñÔ±µÇÂ¼
class UserLogin : public Transaction
{
public:
	virtual void Execute(BankSession& session);
};

}


#endif // _USER_LOGIN_H_
