#include "MainMenuForm.h"

#include "FormManager.h"

#include "../JFC/JMessageBox.h"

using namespace UI;

MainMenuForm::MainMenuForm()
	: JForm(0, 0, 0, 0)
{
}

MainMenuForm::~MainMenuForm()
{
	std::vector<JLinkText*>::const_iterator it;
	for (it=items_.begin(); it!=items_.end(); ++it)
		delete *it;
}

MainMenuForm::MainMenuForm(SHORT x, SHORT y, SHORT w, SHORT h)
	: JForm(x, y, w, h)
{
	char* texts[] =
	{
		"1. Open Account",
		"2. To Deposit Money",
		"3. To Withdraw Deposited Money",
		"4. To Transfer Of Account",
		"5. Balance Inquiry",
		"6. Change Password Of Account",
		"7. Detailed Statement",
		"8. Close Account ",
		"0. Exit"
	};

	for(int i=0; i<9; i++)
	{
		items_.push_back(new JLinkText(22, i*2+5, 40, 1, texts[i], this));
	}
}

void MainMenuForm::Draw()
{
	DrawBorder();
	SetTextColor(FCOLOR_BLUE);
	SetBkColor(BCOLOR_WHITE);
	DrawText(33, 2, "Main Menu List");
	SetTextColor(FCOLOR_BLUE);
	DrawHLine(3, 25, 54, '-');

	JForm::Draw();
}

void MainMenuForm::DrawBorder()
{
	SetTextColor(FCOLOR_YELLO);
	SetBkColor(BCOLOR_RED);
	DrawHLine(0, 0, Width()-1, '-');
	DrawHLine(Height()-1, 0, Width()-1, '-');
	DrawVLine(0, 1, Height()-2, ' ');
	DrawVLine(Width()-1, 1, Height()-2, ' ');
}



void MainMenuForm::OnKeyEvent(JEvent* e)
{
	int key = e->GetEventCode();
	switch (key)
	{
	case KEY_(1):
		e->Done();
		OpenAccount();
		break;
	case KEY_(2):
		e->Done();
		Deposit();
		break;
	case KEY_(3):
		e->Done();
		Withdraw();
		break;
	case KEY_(4):
		e->Done();
		Transfer();
		break;
	case KEY_(5):
		e->Done();
		BalanceInquiry();
		break;
	case KEY_(6):
		e->Done();
		ChangePassword();
		break;
	case KEY_(7):
		e->Done();
		DetailStatement();
		break;
	case KEY_(8):
		e->Done();
		CloseAccount();
		break;
	case KEY_(0):
	case KEY_ESC:
		{
			e->Done();
			std::vector<std::string> v;
			v.push_back(" YES ");
			v.push_back(" NO ");
			std::string msg = "Do you want to exit?";

			JWindow* curr = jApp->GetCurrent();

			int result = JMessageBox::Show("-MESSAGE-", msg,v);
			if (result == 0)
			{
				ClearWindow();
				Quit();
				return;
			}

			ClearWindow();
			curr->SetCurrent();
			Show();
		}
		
		break;
	case KEY_ENTER:
		if (e->GetSender() == items_[0])
		{
			OpenAccount();
			e->Done();
		}
		else if (e->GetSender() == items_[1])
		{
			Deposit();
			e->Done();
		}
		else if (e->GetSender() == items_[2])
		{
			Withdraw();
			e->Done();
		}
		else if (e->GetSender() == items_[3])
		{
			Transfer();
			e->Done();
		}
		else if (e->GetSender() == items_[4])
		{
			BalanceInquiry();
			e->Done();
		}
		else if (e->GetSender() == items_[5])
		{
			ChangePassword();
			e->Done();
		}
		else if (e->GetSender() == items_[6])
		{
			DetailStatement();
			e->Done();
		}
		else if (e->GetSender() == items_[7])
		{
			CloseAccount();
			e->Done();
		}
		else if (e->GetSender() == items_[8])
		{
			std::vector<std::string> v;
			v.push_back(" YES ");
			v.push_back(" NO ");
			std::string msg = "Do you want to exit?";

			JWindow* curr = jApp->GetCurrent();

			int result = JMessageBox::Show("-MESSAGE-", msg,v);
			if (result == 0)
			{
				ClearWindow();
				Quit();
				return;
			}

			ClearWindow();
			curr->SetCurrent();
			Show();

			e->Done();
		}
		break;
	}

	JForm::OnKeyEvent(e);
}

void MainMenuForm::OpenAccount()
{
	//std::vector<std::string> v;
	//v.push_back(" OK ");
	//std::string msg = "OpenAccount";
	//JMessageBox::Show("-MESSAGE-", msg, v);

	JForm* form = Singleton<FormManager>::Instance().Get("OpenAccountForm");
	form->Show();
}

void MainMenuForm::Deposit()
{
	//std::vector<std::string> v;
	//v.push_back(" OK ");
	//std::string msg = "Deposit";
	//JMessageBox::Show("-MESSAGE-", msg, v);

	JForm* form = Singleton<FormManager>::Instance().Get("DepositForm");
	form->Show();
}

void MainMenuForm::Withdraw()
{
	//std::vector<std::string> v;
	//v.push_back(" OK ");
	//std::string msg = "Withdraw";
	//JMessageBox::Show("-MESSAGE-", msg, v);

	JForm* form = Singleton<FormManager>::Instance().Get("WithdrawalForm");
	form->Show();
}

void MainMenuForm::Transfer()
{
	//std::vector<std::string> v;
	//v.push_back(" OK ");
	//std::string msg = "Transfer";
	//JMessageBox::Show("-MESSAGE-", msg, v);

	JForm* form = Singleton<FormManager>::Instance().Get("TransferForm");
	form->Show();
}

void MainMenuForm::BalanceInquiry()
{
	//std::vector<std::string> v;
	//v.push_back(" OK ");
	//std::string msg = "BalanceInquiry";
	//JMessageBox::Show("-MESSAGE-", msg, v);

	JForm* form = Singleton<FormManager>::Instance().Get("BalanceInquiryForm");
	form->Show();
}

void MainMenuForm::ChangePassword()
{
	//std::vector<std::string> v;
	//v.push_back(" OK ");
	//std::string msg = "Deposit";
	//JMessageBox::Show("-MESSAGE-", msg, v);

	JForm* form = Singleton<FormManager>::Instance().Get("ChangePasswordForm");
	form->Show();
}

void MainMenuForm::DetailStatement()
{
	//std::vector<std::string> v;
	//v.push_back(" OK ");
	//std::string msg = "DetailStatement";
	//JMessageBox::Show("-MESSAGE-", msg, v);

	JForm* form = Singleton<FormManager>::Instance().Get("DetailStatementForm");
	form->Show();
}

void MainMenuForm::CloseAccount()
{
	//std::vector<std::string> v;
	//v.push_back(" OK ");
	//std::string msg = "CloseAccount";
	//JMessageBox::Show("-MESSAGE-", msg, v);

	JForm* form = Singleton<FormManager>::Instance().Get("CloseAccountForm");
	form->Show();
}

void MainMenuForm::Quit()
{
	// TODO：返回到登录界面

	JForm* form = Singleton<FormManager>::Instance().Get("LoginForm");
	form->Show();
}