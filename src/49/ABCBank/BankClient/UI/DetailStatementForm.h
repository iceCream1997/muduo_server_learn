#ifndef _DETAIL_STATEMENT_FORM_H_
#define _DETAIL_STATEMENT_FORM_H_

#include "../JFC/JForm.h"
#include "../JFC/JLabel.h"
#include "../JFC/JEdit.h"
#include "../JFC/JButton.h"

#include <string>

using namespace JFC;

namespace UI
{

class DetailStatementForm : public JForm
{
public:
	DetailStatementForm();
	~DetailStatementForm();
	DetailStatementForm(SHORT x, SHORT y, SHORT w, SHORT h,
		const std::string& title);

	virtual void Draw();
	virtual void OnKeyEvent(JEvent* e);

	JButton* btnDetail1_;
	JButton* btnDetail2_;
	JButton* btnDetail3_;
	JButton* btnReturn_;

private:
	void DrawBorder();

	std::string title_;

	
};

}
#endif // _DETAIL_STATEMENT_FORM_H_