#ifndef _QUERY_DAY_BILL_FORM_H_
#define _QUERY_DAY_BILL_FORM_H_

#include "../JFC/JForm.h"
#include "../JFC/JEdit.h"
#include "../JFC/JButton.h"

using namespace JFC;

namespace UI
{

class QueryDayBillForm : public JForm
{
public:
	QueryDayBillForm();
	~QueryDayBillForm();
	QueryDayBillForm(SHORT x, SHORT y, SHORT w, SHORT h);

	virtual void Draw();
	virtual void OnKeyEvent(JEvent* e);

private:
	void DrawBorder();
	void Query();
	void Reset();

	JEdit* editDate_;
	JButton* btnQuery_;
	JButton* btnCancel_;
};

}

#endif // _QUERY_DAY_BILL_FORM_H_