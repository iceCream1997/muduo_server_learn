#include "ReceiptForm.h"

#include "FormManager.h"
#include "MainMenuForm.h"
#include "Validator.h"

#include "../BankSession.h"
#include "../TransactionManager.h"

#include "../JFC/JMessageBox.h"

#include "../../Public/Exception.h"

using namespace UI;

ReceiptForm::ReceiptForm()
	: JForm(0, 0, 0, 0),
	  rft_(RFT_UNKNOWN)
{
}

ReceiptForm::~ReceiptForm()
{
}

ReceiptForm::ReceiptForm(SHORT x, SHORT y, SHORT w, SHORT h,
			const std::vector<std::string>& items,
			const std::string& title/* = std::string()*/)
	: JForm(x, y, w, h),
	  rft_(RFT_UNKNOWN),
	  title_(title)
{

	int yy = y + 4;
	int xx = x + 5;
	std::vector<std::string>::const_iterator it;
	for (it = items.begin(); it != items.end(); ++it)
	{
		JLabel* label;
		label = new JLabel(xx, yy, 13, 1, *it, this);
		yy += 2;

		items_[*it] = label;
	}

	btnReturn_ = new JButton(x + w - 15, yy-4, 10, 3, "·µ  »Ø",this);


}

void ReceiptForm::Draw()
{
	DrawBorder();
	SetTextColor(FCOLOR_BLUE);
	SetBkColor(BCOLOR_CYAN);
	JRECT rect = { 1, 1, Width()-2, Height()-2 };
	FillRect(rect);

	DrawText(5, 2, title_);
	DrawHLine(3, 2, Width()-3, '-');

	JForm::Draw();
}

void ReceiptForm::OnKeyEvent(JEvent* e)
{
	int key = e->GetEventCode();
	if (key == KEY_ENTER && e->GetSender() == btnReturn_)
	{
		
		JForm* form;
		form = Singleton<FormManager>::Instance().Get("MainMenuForm");
		switch (rft_)
		{
		case RFT_OPEN_ACCOUNT:
			dynamic_cast<MainMenuForm*>(form)->GetItems()[0]->SetCurrent();
			break;
		case RFT_DEPOSIT:
			dynamic_cast<MainMenuForm*>(form)->GetItems()[1]->SetCurrent();
			break;
		case RFT_WITHDRAW:
			dynamic_cast<MainMenuForm*>(form)->GetItems()[2]->SetCurrent();
			break;
		case RFT_TRANSFER:
			dynamic_cast<MainMenuForm*>(form)->GetItems()[3]->SetCurrent();
			break;
		case RFT_BALANCE_INQUIRY:
			dynamic_cast<MainMenuForm*>(form)->GetItems()[4]->SetCurrent();
			break;
		case RFT_CLOSE_ACCOUNT:
			dynamic_cast<MainMenuForm*>(form)->GetItems()[7]->SetCurrent();

			break;

		}
		

		form->ClearWindow();
		//ClearWindow();
		form->Show();
		e->Done();
	}

	JForm::OnKeyEvent(e);
}

void ReceiptForm::DrawBorder()
{
	SetTextColor(FCOLOR_YELLO);
	SetBkColor(BCOLOR_MAGENTA);
	DrawHLine(0, 0, Width()-1, '-');
	DrawHLine(Height()-1, 0, Width()-1, '-');
	DrawVLine(0, 1, Height()-2, ' ');
	DrawVLine(Width()-1, 1, Height()-2, ' ');
}

void ReceiptForm::Reset()
{
	std::map<std::string, JLabel*>::const_iterator it;
	for (it = items_.begin(); it != items_.end(); ++it)
	{
		it->second->ClearWindow();
	}

	title_ = "";
}

void ReceiptForm::SetTitle(const std::string& title)
{
	title_ = title;
}

void ReceiptForm::SetItemText(std::string name, std::string text)
{
	items_[name]->SetText(name+":  "+text);
}