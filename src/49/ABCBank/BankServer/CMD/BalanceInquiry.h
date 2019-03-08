#ifndef _BALANCE_INQUIRY_H_
#define _BALANCE_INQUIRY_H_

#include "Transaction.h"

namespace CMD
{

// ≤È—Ø”‡∂Ó

class BalanceInquiry : public Transaction
{
public:
	virtual void Execute(BankSession& session);
};

}


#endif // _BALANCE_INQUIRY_H_
