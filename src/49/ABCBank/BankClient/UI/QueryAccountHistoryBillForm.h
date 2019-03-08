#ifndef _QUERY_ACCOUNT_HISTORY_BILL_FORM_H_
#define _QUERY_ACCOUNT_HISTORY_BILL_FORM_H_

#include "../JFC/JForm.h"
#include "../JFC/JEdit.h"
#include "../JFC/JButton.h"

using namespace JFC;

namespace UI
{

class QueryAccountHistoryBillForm : public JForm
{
public:
	QueryAccountHistoryBillForm();
	~QueryAccountHistoryBillForm();
	QueryAccountHistoryBillForm(SHORT x, SHORT y, SHORT w, SHORT h);

	virtual void Draw();
	virtual void OnKeyEvent(JEvent* e);

private:
	void DrawBorder();
	void Query();
	void Reset();

	JEdit* editAccountId_;
	JEdit* editBeginDate_;
	JEdit* editEndDate_;
	JButton* btnQuery_;
	JButton* btnCancel_;
};

}

#endif // _QUERY_ACCOUNT_HISTORY_BILL_FORM_H_