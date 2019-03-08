#include "FormManager.h"

#include "LoginForm.h"
#include "MainMenuForm.h"
#include "OpenAccountForm.h"
#include "DepositForm.h"
#include "WithdrawalForm.h"
#include "TransferForm.h"
#include "BalanceInquiryForm.h"
#include "ChangePasswordForm.h"
#include "DetailStatementForm.h"
#include "CloseAccountForm.h"

#include "QueryDayBillForm.h"
#include "QueryHistoryBillForm.h"
#include "QueryAccountHistoryBillForm.h"

#include "ReceiptForm.h"
#include "ReportForm.h"


using namespace UI;

FormManager::FormManager()
{
	forms_["LoginForm"] = new LoginForm(0, 0, 80, 25);
	forms_["MainMenuForm"] = new MainMenuForm(0, 0, 80, 25);
	forms_["OpenAccountForm"] = new OpenAccountForm(0, 0, 80, 25, "Open Account");
	forms_["DepositForm"] = new DepositForm(0, 0, 80, 25, "Deposit");
	forms_["WithdrawalForm"] = new WithdrawalForm(0, 0, 80, 25, "Withdraw");
	forms_["TransferForm"] = new TransferForm(0, 0, 80, 25, "Transfer");
	forms_["BalanceInquiryForm"] = new BalanceInquiryForm(0, 0, 80, 25, "BalanceInquiry");
	forms_["ChangePasswordForm"] = new ChangePasswordForm(0, 0, 80, 25, "ChangePassword");
	forms_["DetailStatementForm"] = new DetailStatementForm(0, 0, 80, 25, "Detail Statement");
	forms_["CloseAccountForm"] = new CloseAccountForm(0, 0, 80, 25, "Close Account");
	forms_["QueryDayBillForm"] = new QueryDayBillForm(13, 5, 54, 13);
	forms_["QueryHistoryBillForm"] = new QueryHistoryBillForm(13, 5, 54, 13);
	forms_["QueryAccountHistoryBillForm"] = new QueryAccountHistoryBillForm(13, 5, 54, 15);

	forms_["ReportForm"] = new ReportForm(0, 0, 80, 25);

	char* items[] =
	{
		"开户日期",
		"户    名",
		"帐    号",
		"金    额",
	};
	std::vector<std::string> v(items, items+4);
	// 开户回单
	forms_["OpenAccountReceiptForm"] = new ReceiptForm(10, 6, 60, 13, v, "开户成功");


	char* items2[] =
	{
		"交易日期",
		"户    名",
		"帐    号",
		"交易金额",
		"摘    要",
		"余    额",
	};
	std::vector<std::string> v2(items2, items2+6);
	// 存款、取款回单
	forms_["ReceiptForm"] = new ReceiptForm(10, 4, 60, 17, v2);

	char* items3[] =
	{
		"交易日期",
		"户    名",
		"帐    号",
		"对方帐号",
		"交易金额",
		"摘    要",
		"余    额",
	};
	std::vector<std::string> v3(items3, items3+7);
	// 转帐回单
	forms_["TransferReceiptForm"] = new ReceiptForm(10, 3, 60, 19, v3, "转帐成功");

	char* items4[] =
	{
		"交易日期",
		"户    名",
		"帐    号",
		"余    额",
	};
	std::vector<std::string> v4(items4, items4+4);
	// 余额查询回单
	forms_["BalanceInquiryReceiptForm"] = new ReceiptForm(10, 6, 60, 13, v4, "余额查询成功");

	char* items5[] =
	{
		"销户日期",
		"户    名",
		"帐    号",
		"余    额",
		"利    息",
		"总    计",
	};
	std::vector<std::string> v5(items5, items5+6);
	// 销户回单
	forms_["CloseAccountReceiptForm"] = new ReceiptForm(10, 4, 60, 17, v5);
}

FormManager::~FormManager()
{
}
