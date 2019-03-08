#ifndef _TRANSACTION_MANAGER_H_
#define _TRANSACTION_MANAGER_H_

#include "../Public/Singleton.h"
#include "BankSession.h"
#include "CMD/Transaction.h"
#include <map>
#include <string>
using namespace std;

using namespace CMD;

class TransactionManager
{
	friend class Singleton<TransactionManager>;
public:
	bool DoAction(BankSession& session);

private:
	map<uint16, Transaction*> m_actions;
	TransactionManager();
	TransactionManager(const TransactionManager& rhs);
	~TransactionManager();

};

#endif // _TRANSACTION_MANAGER_H_
