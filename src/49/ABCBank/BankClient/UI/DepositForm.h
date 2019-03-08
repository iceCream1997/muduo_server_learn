#ifndef _DEPOSIT_FORM_H_
#define _DEPOSIT_FORM_H_

#include "../JFC/JForm.h"
#include "../JFC/JLabel.h"
#include "../JFC/JEdit.h"
#include "../JFC/JButton.h"

#include <string>

using namespace JFC;

namespace UI
{

class DepositForm : public JForm
{
public:
	DepositForm();
	~DepositForm();
	DepositForm(SHORT x, SHORT y, SHORT w, SHORT h,
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

	JLabel* lblMoney_;
	JEdit* editMoney_;
	JLabel* lblMoneyTip_;

	JButton* btnSubmit_;
	JButton* btnReset_;
	JButton* btnCancel_;
};

}
#endif // _DEPOSIT_FORM_H_