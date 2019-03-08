#ifndef _IBANK_SERVICE_H_
#define _IBANK_SERVICE_H_

#include "Account.h"
#include "TransDetail.h"

#include <list>
using namespace std;

namespace DAL
{

class IBankService
{
public:
	// 用户登录
	virtual int UserLogin(const string& user, const string& pass) = 0;
	// 开户
	virtual int OpenAccount(Account& account) = 0;
	// 销户
	virtual int CloseAccount(Account& account, double& interest) = 0;
	// 修改密码
	virtual int ChangePassword(Account& account, const string& newPass) = 0;
	// 存款
	virtual int Deposit(Account& account) = 0;
	// 取款
	virtual int WithDraw(Account& account) = 0;
	// 转帐
	virtual int Transfer(Account& account, const string& otherAccountId) = 0;
	// 查询余额
	virtual int BalanceInquiry(Account& account) = 0;
	// 查询某日报表
	virtual int QueryDayBill(list<TransDetail>& result, int page, const string& date) = 0;
	// 根据日期查询明细
	virtual int QueryHistoryBill(list<TransDetail>& result, int page, const string& begin, const string& end) = 0;
	// 根据帐号查询明细
	virtual int QueryAccountHistoryBill(list<TransDetail>& result, int page, const string& accountId, const string& begin, const string& end) = 0;
};

}
#endif /* _IBANK_SERVICE_H_ */