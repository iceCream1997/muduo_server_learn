#ifndef _CHANGE_PASSWORD_H_
#define _CHANGE_PASSWORD_H_

#include "Transaction.h"

namespace CMD
{

// ÐÞ¸ÄÃÜÂë

class ChangePassword : public Transaction
{
public:
	virtual void Execute(BankSession& session);
};

}

#endif // _CHANGE_PASSWORD_H_
