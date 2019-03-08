#ifndef _MAIN_MENU_FORM_H_
#define _MAIN_MENU_FORM_H_

#include "../JFC/JForm.h"
#include "../JFC/JLinkText.h"
#include "../JFC/JEvent.h"

#include <vector>

using namespace JFC;

namespace UI
{

class MainMenuForm : public JForm
{
public:
	MainMenuForm();
	~MainMenuForm();
	MainMenuForm(SHORT x, SHORT y, SHORT w, SHORT h);

	virtual void Draw();
	virtual void OnKeyEvent(JEvent* e);

	void OpenAccount();
	void Deposit();
	void Withdraw();
	void Transfer();
	void BalanceInquiry();
	void ChangePassword();
	void DetailStatement();
	void CloseAccount();
	void Quit();

	std::vector<JLinkText*>& GetItems() { return items_; }


private:
	void DrawBorder();

	std::vector<JLinkText*> items_;

};

}
#endif // _MAIN_MENU_FORM_H_