#ifndef _RECEIPT_FORM_H_
#define _RECEIPT_FORM_H_

#include "../JFC/JForm.h"
#include "../JFC/JLabel.h"
#include "../JFC/JEdit.h"
#include "../JFC/JButton.h"
#include "../JFC/JEvent.h"

#include <string>
#include <vector>
#include <map>

using namespace JFC;

namespace UI
{

class ReceiptForm : public JForm
{
public:
	enum ReceiptFormType
	{
		RFT_UNKNOWN,
		RFT_OPEN_ACCOUNT,
		RFT_DEPOSIT,
		RFT_WITHDRAW,
		RFT_TRANSFER,
		RFT_BALANCE_INQUIRY,
		RFT_CLOSE_ACCOUNT
	};
	ReceiptForm();
	~ReceiptForm();
	ReceiptForm(SHORT x, SHORT y, SHORT w, SHORT h,
		const std::vector<std::string>& items,
		const std::string& title = std::string());

	virtual void Draw();
	virtual void OnKeyEvent(JEvent* e);

	void SetItemText(std::string name, std::string text);
	void SetTitle(const std::string& title);

	void SetReceiptFormType(ReceiptFormType rft)
	{
		rft_ = rft;
	}
private:
	void DrawBorder();

	void Reset();

	ReceiptFormType rft_;
	std::string title_;
	std::map<std::string, JLabel*> items_;
	//std::map<std::string, std::string> texts_;

	JButton* btnReturn_;
};

}
#endif // _RECEIPT_FORM_H_