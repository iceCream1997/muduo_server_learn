#ifndef _QUERY_DAY_BILL_H_
#define _QUERY_DAY_BILL_H_

#include "Transaction.h"

namespace CMD
{

	class QueryDayBill : public Transaction
	{
	public:
		virtual void Execute(BankSession& session);
	};

}

#endif // _QUERY_DAY_BILL_H_