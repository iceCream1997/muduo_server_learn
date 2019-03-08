#include "JForm.h"

using namespace JFC;

JForm::JForm()
	: JWindow(0, 0, 0, 0, 0)
{
}

JForm::JForm(SHORT x, SHORT y, SHORT width, SHORT height,
				 JWindow* parent)
	: JWindow(x, y, width, height, parent)
{

}

JForm::~JForm()
{
}

void JForm::Draw()
{
	// 遍历子窗口列表，对各个子窗口进行绘制
	std::vector<JWindow*>::const_iterator it;
	for (it=childs_.begin(); it!=childs_.end(); ++it)
	{
		(*it)->Draw();
	}
}

void JForm::OnKeyEvent(JEvent* e)
{
	if (e->IsDone())
		return;

	if (e->GetSender() == this)
		return;

	int key = e->GetEventCode();
	JWindow* win;

	switch (key)
	{
	case KEY_UP:
	case KEY_LEFT:
		e->Done();
		win = FindPrev(e->GetSender());
		if (win == NULL)
			return;

		win->Show();

		(e->GetSender())->Draw();
		(e->GetSender())->Refresh();
		break;

	case KEY_ENTER:
	case KEY_DOWN:
	case KEY_RIGHT:
	case KEY_TAB:
		e->Done();
		win = FindNext(e->GetSender());
		if (win == NULL)
			return;

		win->Show();

		(e->GetSender())->Draw();
		(e->GetSender())->Refresh();
		break;

	default:
		break;

	}

	//JWindow* win = FindNext()
}