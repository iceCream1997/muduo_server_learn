#ifndef _QUERY_ACCOUNT_HISTORY_BILL_H_
#define _QUERY_ACCOUNT_HISTORY_BILL_H_

#include "Transaction.h"

namespace CMD
{

class QueryAccountHistoryBill : public Transaction
{
public:
	virtual void Execute(BankSession& session);
};

}

#endif // _QUERY_ACCOUNT_HISTORY_BILL_H_
