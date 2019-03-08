#include "ChangePasswordForm.h"

#include "FormManager.h"
#include "MainMenuForm.h"
#include "Validator.h"

#include "../BankSession.h"
#include "../TransactionManager.h"

#include "../JFC/JMessageBox.h"

#include "../../Public/Exception.h"

using namespace UI;

ChangePasswordForm::ChangePasswordForm()
	: JForm(0, 0, 0, 0, 0)
{
}

ChangePasswordForm::~ChangePasswordForm()
{
}

ChangePasswordForm::ChangePasswordForm(SHORT x, SHORT y, SHORT w, SHORT h,
									   const std::string& title)
	: JForm(x, y, w, h),
	  title_(title)
{

	lblAccountId_ = new JLabel(14, 5, 11, 1, "ACCOUNT ID:", this);
	editAccountId_ = new JEdit(26, 5, 21, 1, "", this);
	lblAccountIdTip_ = new JLabel(50, 5, 11, 1, "长度6位，数字", this);

	lblPass_ = new JLabel(8, 8, 17, 1, "ACCOUNT PASSWORD:", this);
	editPass_ = new JEdit(26, 8, 21, 1, "", this, JEdit::EM_PASSWORD);
	lblPassTip_ = new JLabel(50, 8, 11, 1, "长度6-8位", this);

	lblNewPass_ = new JLabel(12, 11, 13, 1, "NEW PASSWORD:", this);
	editNewPass_ = new JEdit(26, 11, 21, 1, "", this, JEdit::EM_PASSWORD);
	lblNewPassTip_ = new JLabel(50, 11, 11, 1, "长度6-8位", this);


	lblNewPass2_ = new JLabel(9, 14, 16, 1, "REPEAT PASSWORD:", this);
	editNewPass2_ = new JEdit(26, 14, 21, 1, "", this, JEdit::EM_PASSWORD);
	lblNewPass2Tip_ = new JLabel(50, 14, 7, 1, "同上", this);

	btnSubmit_ = new JButton(5, 20, 10, 3, "SUBMIT", this);
	btnReset_ =	new JButton(50, 20, 10, 3,"RESET", this);
	btnCancel_ = new JButton(65, 20, 10, 3,"CANCEL", this);

	editAccountId_->SetValidator(ValidateAccountId);
	editPass_->SetValidator(ValidatePass);
	editNewPass_->SetValidator(ValidatePass);
	editNewPass2_->SetValidator(ValidatePass);


}

void ChangePasswordForm::Draw()
{
	DrawBorder();
	SetTextColor(FCOLOR_BLUE);
	SetBkColor(BCOLOR_WHITE);
	JRECT rect = { 1, 1, Width()-2, Height()-2 };
	FillRect(rect);

	DrawText(5, 2, title_);
	DrawHLine(3, 2, Width()-3, '-');

	JForm::Draw();
}

void ChangePasswordForm::DrawBorder()
{
	SetTextColor(FCOLOR_YELLO);
	SetBkColor(BCOLOR_RED);
	DrawHLine(0, 0, Width()-1, '-');
	DrawHLine(Height()-1, 0, Width()-1, '-');
	DrawVLine(0, 1, Height()-2, ' ');
	DrawVLine(Width()-1, 1, Height()-2, ' ');
}

void ChangePasswordForm::OnKeyEvent(JEvent* e)
{
	int key = e->GetEventCode();
	if (key == KEY_ESC)
	{
		Reset();
		JForm* form;
		form = Singleton<FormManager>::Instance().Get("MainMenuForm");
		dynamic_cast<MainMenuForm*>(form)->GetItems()[5]->SetCurrent();
		form->ClearWindow();
		form->Show();
		e->Done();
	}
	else if (key == KEY_ENTER)
	{
		if (e->GetSender() == btnCancel_)
		{
			Reset();
			JForm* form;
			form = Singleton<FormManager>::Instance().Get("MainMenuForm");
			dynamic_cast<MainMenuForm*>(form)->GetItems()[5]->SetCurrent();
			form->ClearWindow();
			form->Show();
			e->Done();
		}
		else if (e->GetSender() == btnReset_)
		{
			Reset();
			editAccountId_->SetCurrent();
			Show();
			e->Done();
		}
		else if (e->GetSender() == btnSubmit_)
		{
			Submit();
			e->Done();
		}
	}

	JForm::OnKeyEvent(e);
}

void ChangePasswordForm::Reset()
{
	editAccountId_->SetText("");
	editPass_->SetText("");
	editNewPass_->SetText("");
	editNewPass2_->SetText("");
}

void ChangePasswordForm::Submit()
{
	if (editAccountId_->GetText().length() < 6)
	{
		std::vector<std::string> v;
		v.push_back(" YES ");
		std::string msg = "帐号小于6位";

		JMessageBox::Show("-ERROR-", msg,v);
		ClearWindow();
		Show();
		editAccountId_->Show();
		return;
	}
	if (editPass_->GetText().length() < 6)
	{
		std::vector<std::string> v;
		v.push_back(" YES ");
		std::string msg = "密码小于6位";

		JMessageBox::Show("-ERROR-", msg,v);
		ClearWindow();
		Show();
		editPass_->Show();
		return;
	}
	if (editNewPass_->GetText() != editNewPass2_->GetText())
	{
		std::vector<std::string> v;
		v.push_back(" YES ");
		std::string msg = "确认密码不一致";

		JMessageBox::Show("-ERROR-", msg,v);
		ClearWindow();
		Show();
		editNewPass2_->Show();
		return;
	}

	if (editPass_->GetText() == editNewPass_->GetText())
	{
		std::vector<std::string> v;
		v.push_back(" YES ");
		std::string msg = "新密码与原密码不能相同";

		JMessageBox::Show("-ERROR-", msg,v);
		ClearWindow();
		Show();
		editNewPass_->Show();
		return;
	}

	// 以下为实际的修改密码操作
	try
	{
		BankSession bs;
		bs.SetCmd(CMD_CHANGE_PASSWORD);
		bs.SetAttribute("account_id", editAccountId_->GetText());
		bs.SetAttribute("pass", editPass_->GetText());
		bs.SetAttribute("newpass", editNewPass_->GetText());

		Singleton<TransactionManager>::Instance().DoAction(bs);
		if (bs.GetErrorCode() == 0)
		{
			Reset();
			std::vector<std::string> v;
			v.push_back(" YES ");
			std::string msg = "修改密码成功";

			JMessageBox::Show("-MESSAGE-", msg,v);


			JForm* form = Singleton<FormManager>::Instance().Get("MainMenuForm");
			dynamic_cast<MainMenuForm*>(form)->GetItems()[5]->SetCurrent();
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