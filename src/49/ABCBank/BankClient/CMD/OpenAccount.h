#ifndef _OPEN_ACCOUNT_H_
#define _OPEN_ACCOUNT_H_

#include "Transaction.h"

namespace CMD
{

// ¿ª»§
class OpenAccount : public Transaction
{
public:
	virtual void Execute(BankSession& session);
};

}

#endif // _OPEN_ACCOUNT_H_
