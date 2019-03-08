#ifndef _LOGIN_FORM_H_
#define _LOGIN_FORM_H_

#include "../JFC/JForm.h"
#include "../JFC/JButton.h"
#include "../JFC/JLabel.h"
#include "../JFC/JEdit.h"

using namespace JFC;

namespace UI
{


class LoginForm : public JForm
{
public:
	LoginForm(SHORT x, SHORT y, SHORT width, SHORT height,
		JWindow* parent = jApp->Root());

	~LoginForm();

	virtual void Draw();
	virtual void OnKeyEvent(JEvent* e);

private:
	void DrawBorder();

	void Login();
	void Exit();
	void Reset();

	JEdit* editUser_;
	JLabel* lblUser_;
	JLabel* lblUserTip_;

	JEdit* editPass_;
	JLabel* lblPass_;
	JLabel* lblPassTip_;

	JButton* btnLogin_;
	JButton* btnExit_;
};

}

#endif // _LOGIN_FORM_H_