#include "TransactionManager.h"
#include "CMD/UserLogin.h"
#include "CMD/OpenAccount.h"
#include "CMD/CloseAccount.h"
#include "CMD/ChangePassword.h"
#include "CMD/Deposit.h"
#include "CMD/BalanceInquiry.h"
#include "CMD/Transfer.h"
#include "CMD/Withdrawal.h"
#include "CMD/QueryDayBill.h"
#include "CMD/QueryHistoryBill.h"
#include "CMD/QueryAccountHistoryBill.h"



TransactionManager::TransactionManager()
{
	m_actions[CMD_LOGIN] = new UserLogin;
	m_actions[CMD_OPEN_ACCOUNT] = new OpenAccount;
	m_actions[CMD_DEPOSIT] = new Deposit;
	m_actions[CMD_WITHDRAW] = new Withdrawal;
	m_actions[CMD_TRANSFER] = new Transfer;
	m_actions[CMD_BALANCE_INQUIRY] = new BalanceInquiry;
	m_actions[CMD_CHANGE_PASSWORD] = new ChangePassword;
	m_actions[CMD_DAY_BILL] = new QueryDayBill;
	m_actions[CMD_HISTORY_BILL] = new QueryHistoryBill;
	m_actions[CMD_ACCOUNT_HISTORY_BILL] = new QueryAccountHistoryBill;
	m_actions[CMD_CLOSE_ACCOUNT] = new CloseAccount;
}

TransactionManager::~TransactionManager()
{
	for (map<uint16, Transaction*>::iterator it=m_actions.begin();
		it!=m_actions.end();++it)
	{
			delete it->second;
	}
}

bool TransactionManager::DoAction(BankSession& session)
{
	uint16 cmd = session.GetCmd();
	if (m_actions.find(cmd) != m_actions.end())
	{
		m_actions[cmd]->Execute(session);
		return true;
	}

	return false;
}