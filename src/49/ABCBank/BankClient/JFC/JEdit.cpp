#include "JEdit.h"
#include "JEvent.h"

using namespace JFC;

JEdit::JEdit()
	: currIndex_(0),
	  mode_(EM_NORMAL),
	  validator_(DefaultValidator)
{
	SetCanFocus(true);
}

JEdit::JEdit(SHORT x, SHORT y, SHORT width, SHORT height,
			 const std::string& text,
			 JWindow* parent,
			 EchoMode mode,
			 VALIDATOR validator)
	: JWindow(x, y, width, height, parent),
	  text_(text),
	  currIndex_(0),
	  mode_(mode),
	  validator_(validator)
{
	SetCanFocus(true);
}

JEdit::~JEdit()
{

}

void JEdit::OnKeyEvent(JEvent* e)
{
	int key = e->GetEventCode();
	switch (key)
	{
	case KEY_RIGHT:
		if (currIndex_  < static_cast<int>(text_.length()))
		{
			++currIndex_;
			DrawText(0, 0, text_);
			Show();
			e->Done();
		}
		break;
	case KEY_LEFT:
		if (currIndex_ > 0)
		{
			--currIndex_;
			DrawText(0, 0, text_);
			Show();
			e->Done();
		}
		break;
	case KEY_DEL:
		text_.erase(text_.begin()+currIndex_);
		ClearWindow();
		DrawText(0, 0, text_);
		Show();
		e->Done();

		break;
	case KEY_BACK:

		if (currIndex_ > 0)
		{
			text_.erase(text_.begin()+currIndex_-1);
			--currIndex_;
		}

		ClearWindow();
		DrawText(0, 0, text_);
		Show();
		e->Done();

		break;

	case KEY_ENTER:
	case KEY_DOWN:
	case KEY_TAB:
	case KEY_UP:
		break;

	default:
		if (key > 0 && key < 256 &&
			isprint(key) &&
			static_cast<int>(text_.length()) < Width()-1 &&
			validator_(text_, (char)e->GetEventCode()))
		{
			text_.insert(text_.begin()+currIndex_, 1, (char)e->GetEventCode());
			++currIndex_;
			DrawText(0, 0, text_);
			Show();
			e->Done();
		}
		break;
	}

	if (!e->IsDone())
	{
		if (parent_ != NULL)
			parent_->OnKeyEvent(e);
	}
}
void JEdit::Draw()
{
	if (IsCurrent())
	{
		SetTextColor(FCOLOR_WHITE);
		SetBkColor(BCOLOR_RED);
		jApp->ShowCursor();
		SetCursorPos(x_ + currIndex_, y_);
	}
	else
	{
		SetTextColor(FCOLOR_WHITE);
		SetBkColor(BCOLOR_BLUE);
	}
	JRECT rect = { 0, 0, Width()-1, Height()-1 };
	FillRect(rect);
	if (mode_ == JEdit::EM_NORMAL)
		DrawText(0, 0, text_);
	else
	{
		std::string s(text_.length(), '*');
		DrawText(0, 0, s);
	}
}


