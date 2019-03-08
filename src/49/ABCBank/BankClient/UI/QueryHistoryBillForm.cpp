#include "QueryHistoryBillForm.h"

#include "FormManager.h"
#include "DetailStatementForm.h"
#include "ReportForm.h"
#include "Validator.h"

#include "../BankSession.h"
#include "../TransactionManager.h"

#include "../JFC/JMessageBox.h"

#include "../../Public/Exception.h"

using namespace UI;

QueryHistoryBillForm::QueryHistoryBillForm()
	: JForm(0, 0, 0, 0, 0)
{
}

QueryHistoryBillForm::~QueryHistoryBillForm()
{
}

QueryHistoryBillForm::QueryHistoryBillForm(SHORT x, SHORT y, SHORT w, SHORT h)
	: JForm(x, y, w, h)
{
	editBeginDate_ = new JEdit(29, 9, 20, 1, "", this);
	editEndDate_ = new JEdit(29, 11, 20, 1, "", this);
	btnQuery_ = new JButton(28, 13, 11, 3, "QUERY", this);
	btnCancel_ = new JButton(41, 13, 10, 3, "CANCEL", this);

	editBeginDate_->SetValidator(ValidateDate);
	editEndDate_->SetValidator(ValidateDate);
}

void QueryHistoryBillForm::Draw()
{
	DrawBorder();

	SetTextColor(FCOLOR_BLUE);
	SetBkColor(BCOLOR_WHITE);

	DrawText(3, 2, "-DETAIL QUERY-");
	DrawText(4, 4, "Begin Date:");
	DrawText(39, 4, "YYYY-MM-DD");

	DrawText(6, 6, "End Date:");
	DrawText(39, 6, "YYYY-MM-DD");

	JForm::Draw();
}

void QueryHistoryBillForm::DrawBorder()
{
	SetTextColor(FCOLOR_YELLO);
	SetBkColor(BCOLOR_RED);
	DrawHLine(0, 0, Width()-1, '-');
	DrawHLine(Height()-1, 0, Width()-1, '-');
	DrawVLine(0, 1, Height()-2, ' ');
	DrawVLine(Width()-1, 1, Height()-2, ' ');
}

void QueryHistoryBillForm::OnKeyEvent(JEvent* e)
{
	int key = e->GetEventCode();
	if (key == KEY_ESC)
	{
		JForm* form;
		ClearWindow();
		form = Singleton<FormManager>::Instance().Get("DetailStatementForm");
		dynamic_cast<DetailStatementForm*>(form)->btnDetail2_->SetCurrent();
		form->Show();
		e->Done();
	}
	else if (key == KEY_ENTER)
	{
		if (e->GetSender() == btnCancel_)
		{
			JForm* form;
			ClearWindow();
			form = Singleton<FormManager>::Instance().Get("DetailStatementForm");
			dynamic_cast<DetailStatementForm*>(form)->btnDetail2_->SetCurrent();
			form->Show();
			e->Done();
		}
		if (e->GetSender() == btnQuery_)
		{
			Query();
			e->Done();
		}
	}

	JForm::OnKeyEvent(e);
}

void QueryHistoryBillForm::Query()
{
	if (editBeginDate_->GetText().length() < 10)
	{
		std::vector<std::string> v;
		v.push_back(" YES ");
		std::string msg = "开始日期小于10位";

		int result = JMessageBox::Show("-ERROR-", msg,v);
		ClearWindow();
		Show();
		editBeginDate_->Show();
		return;
	}

	if (editEndDate_->GetText().length() < 10)
	{
		std::vector<std::string> v;
		v.push_back(" YES ");
		std::string msg = "结束日期小于10位";

		int result = JMessageBox::Show("-ERROR-", msg,v);
		ClearWindow();
		Show();
		editEndDate_->Show();
		return;
	}

	// 以下为实际的查询操作
	try
	{
		BankSession bs;
		bs.SetCmd(CMD_HISTORY_BILL);
		bs.SetAttribute("begin_date", editBeginDate_->GetText());
		bs.SetAttribute("end_date", editEndDate_->GetText());
		bs.SetAttribute("page", "0");

		Singleton<TransactionManager>::Instance().DoAction(bs);
		if (bs.GetErrorCode() == 0)
		{
			ReportForm* form;
			form = dynamic_cast<ReportForm*>(Singleton<FormManager>::Instance().Get("ReportForm"));
			form->SetCmd(CMD_HISTORY_BILL);
			list<TransDetail>& tds = bs.GetDetails();
			list<TransDetail>::iterator it = tds.begin();
			form->SetLines(it->total);
			form->SetDetails(bs.GetDetails());
			form->SetBeginDate(editBeginDate_->GetText());
			form->SetEndDate(editEndDate_->GetText());

			form->ClearWindow();
			form->Show();
		}
		else
		{
			std::vector<std::string> v;
			v.push_back(" YES ");

			JMessageBox::Show("-ERROR-", bs.GetErrorMsg(), v);
			ClearWindow();
			Show();
			return;
		}
	}

	catch (Exception& e)
	{
		std::vector<std::string> v;
		v.push_back(" YES ");

		int result = JMessageBox::Show("-ERROR-", e.what(), v);
		ClearWindow();
		Show();

		return;
	}
}

void QueryHistoryBillForm::Reset()
{
	editBeginDate_->SetText("");
	editEndDate_->SetText("");
}