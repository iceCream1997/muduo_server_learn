#ifndef _OPEN_ACCOUNT_FORM_H_
#define _OPEN_ACCOUNT_FORM_H_

#include "../JFC/JForm.h"
#include "../JFC/JLabel.h"
#include "../JFC/JEdit.h"
#include "../JFC/JButton.h"

#include <string>

using namespace JFC;

namespace UI
{

class OpenAccountForm : public JForm
{
public:
	OpenAccountForm();
	~OpenAccountForm();
	OpenAccountForm(SHORT x, SHORT y, SHORT w, SHORT h,
		const std::string& title);

	virtual void Draw();
	virtual void OnKeyEvent(JEvent* e);

private:
	void DrawBorder();

	void Reset();
	void Submit();

	std::string title_;

	JLabel* lblName_;
	JEdit* editName_;
	JLabel* lblNameTip_;

	JLabel* lblId_;
	JEdit* editId_;
	JLabel* lblIdTip_;

	JLabel* lblPass_;
	JEdit* editPass_;
	JLabel* lblPassTip_;

	JLabel* lblPass2_;
	JEdit* editPass2_;
	JLabel* lblPass2Tip_;

	JLabel* lblMoney_;
	JEdit* editMoney_;
	JLabel* lblMoneyTip_;

	JButton* btnSubmit_;
	JButton* btnReset_;
	JButton* btnCancel_;
};

}
#endif // _OPEN_ACCOUNT_FORM_H_