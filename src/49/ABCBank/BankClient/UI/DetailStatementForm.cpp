#include "DetailStatementForm.h"

#include "FormManager.h"
#include "MainMenuForm.h"

#include "QueryDayBillForm.h"
#include "QueryHistoryBillForm.h"
#include "QueryAccountHistoryBillForm.h"

#include "../JFC/JMessageBox.h"

using namespace UI;

DetailStatementForm::DetailStatementForm()
	: JForm(0, 0, 0, 0, 0)
{
}

DetailStatementForm::~DetailStatementForm()
{
}

DetailStatementForm::DetailStatementForm(SHORT x, SHORT y, SHORT w, SHORT h,
										 const std::string& title)
	: JForm(x, y, w, h),
	  title_(title)
{
	btnDetail1_ = new JButton(18, 6, 40, 3, "View the one day's detail", this);
	btnDetail2_ =	new JButton(18, 11, 40, 3,"View the detail in a range", this);
	btnDetail3_ = new JButton(18, 16, 40, 3,"Search the record by account id", this);
	btnReturn_ = new JButton(65, 20, 10, 3,"RETURN", this);
}

void DetailStatementForm::Draw()
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

void DetailStatementForm::DrawBorder()
{
	SetTextColor(FCOLOR_YELLO);
	SetBkColor(BCOLOR_RED);
	DrawHLine(0, 0, Width()-1, '-');
	DrawHLine(Height()-1, 0, Width()-1, '-');
	DrawVLine(0, 1, Height()-2, ' ');
	DrawVLine(Width()-1, 1, Height()-2, ' ');
}

void DetailStatementForm::OnKeyEvent(JEvent* e)
{
	int key = e->GetEventCode();
	if (key == KEY_ESC)
	{
		JForm* form;
		//ClearWindow();
		form = Singleton<FormManager>::Instance().Get("MainMenuForm");
		dynamic_cast<MainMenuForm*>(form)->GetItems()[6]->SetCurrent();
		form->ClearWindow();
		form->Show();
		e->Done();
	}
	else if (key == KEY_ENTER)
	{
		if (e->GetSender() == btnDetail1_)
		{
			JForm* form;
			ClearWindow();
			
			form = Singleton<FormManager>::Instance().Get("QueryDayBillForm");
			form->Show();

			e->Done();
		}
		if (e->GetSender() == btnDetail2_)
		{
			JForm* form;
			//ClearWindow();

			form = Singleton<FormManager>::Instance().Get("QueryHistoryBillForm");
			form->ClearWindow();
			form->Show();

			e->Done();
		}
		if (e->GetSender() == btnDetail3_)
		{

			JForm* form;
			//ClearWindow();

			form = Singleton<FormManager>::Instance().Get("QueryAccountHistoryBillForm");
			form->ClearWindow();
			form->Show();

			e->Done();

		}
		if (e->GetSender() == btnReturn_)
		{
			JForm* form;
			//ClearWindow();
			form = Singleton<FormManager>::Instance().Get("MainMenuForm");
			dynamic_cast<MainMenuForm*>(form)->GetItems()[6]->SetCurrent();
			form->ClearWindow();
			form->Show();
			e->Done();
		}
	}

	JForm::OnKeyEvent(e);
}

