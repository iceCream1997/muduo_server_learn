#ifndef _BANK_SERVICE_H_
#define _BANK_SERVICE_H_

#include "IBankService.h"

namespace DAL
{


class BankService : public IBankService
{
public:
	// 用户登录
	int UserLogin(const string& user, const string& pass);
	// 开户
	int OpenAccount(Account& account);
	// 销户
	int CloseAccount(Account& account, double& interest);
	// 修改密码
	int ChangePassword(Account& account, const string& newPass);
	// 存款
	int Deposit(Account& account);
	// 取款
	int WithDraw(Account& account);
	// 转帐
	int Transfer(Account& account, const string& otherAccountId);
	// 查询余额
	int BalanceInquiry(Account& account);
	// 查询某日报表
	int QueryDayBill(list<TransDetail>& result, int page, const string& date);
	// 根据日期查询明细
	int QueryHistoryBill(list<TransDetail>& result, int page, const string& begin, const string& end);
	// 根据帐号查询明细
	int QueryAccountHistoryBill(list<TransDetail>& result, int page, const string& accountId, const string& begin, const string& end);
};

}
#endif /* _BANK_SERVICE_H_ */