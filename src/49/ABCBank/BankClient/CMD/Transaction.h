#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_

#include "../BankSession.h"

namespace CMD
{

// √¸¡Óª˘¿‡
class Transaction
{
public:
	virtual void Execute(BankSession& session) = 0;
	virtual ~Transaction() {};
};

}

#endif // _TRANSACTION_H_
