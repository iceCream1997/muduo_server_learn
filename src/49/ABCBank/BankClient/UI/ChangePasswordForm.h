#ifndef _CHANGE_PASSWORD_FORM_H_
#define _CHANGE_PASSWORD_FORM_H_

#include "../JFC/JForm.h"
#include "../JFC/JLabel.h"
#include "../JFC/JEdit.h"
#include "../JFC/JButton.h"

#include <string>

using namespace JFC;

namespace UI
{

class ChangePasswordForm : public JForm
{
public:
	ChangePasswordForm();
	~ChangePasswordForm();
	ChangePasswordForm(SHORT x, SHORT y, SHORT w, SHORT h,
		const std::string& title);

	virtual void Draw();
	virtual void OnKeyEvent(JEvent* e);

private:
	void DrawBorder();

	void Reset();
	void Submit();

	std::string title_;

	JLabel* lblAccountId_;
	JEdit* editAccountId_;
	JLabel* lblAccountIdTip_;

	JLabel* lblPass_;
	JEdit* editPass_;
	JLabel* lblPassTip_;

	JLabel* lblNewPass_;
	JEdit* editNewPass_;
	JLabel* lblNewPassTip_;

	JLabel* lblNewPass2_;
	JEdit* editNewPass2_;
	JLabel* lblNewPass2Tip_;

	JButton* btnSubmit_;
	JButton* btnReset_;
	JButton* btnCancel_;
};

}
#endif // _CHANGE_PASSWORD_FORM_H_