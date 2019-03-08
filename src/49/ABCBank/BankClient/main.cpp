#include "Client.h"

#include "JFC/JApplication.h"
#include "JFC/JButton.h"

#include "UI/LoginForm.h"
#include "UI/MainMenuForm.h"

#include "UI/FormManager.h"

#include "../Public/Logging.h"
#include "../Public/JInStream.h"
#include "../Public/JOutStream.h"
#include "../Public/Exception.h"

#include <iostream>


using namespace JFC;
using namespace UI;
using namespace PUBLIC;

/*
int main()
{
	//LOG_INFO<<"Entering main";
	//LOG_INFO<<"Do something";
	//LOG_INFO<<"Exiting main";

	JOutStream jos;
	string s1 = "abc";
	string s2 = "xyz";
	int x = 100;


	jos<<s1<<s2<<x;		// 打包

	JInStream jis(jos.Data(), jos.Length());

	string ss1;
	string ss2;
	int xx;
	jis>>ss1>>ss2>>xx;

	std::cout<<ss1<<" "<<ss2<<" "<<xx<<std::endl;



	return 0;
}
*/


int main()
{
	try
	{
		Singleton<Client>::Instance();
	}
	catch (Exception& e)
	{
		LOG_INFO<<e.what();
		system("pause");
		return 1;
	}
	
	JApplication app;

	//JButton btn(11, 2, 10, 3, "test");
	//btn.Show();

	//JButton btn2(25, 2, 10, 3, "test2");
	//btn2.Show();

	//btn.Draw();		// 将窗口绘制到逻辑屏幕
	//btn.Refresh();	// 从逻辑屏幕映射到物理屏幕


	//LoginForm login(0, 0, 80, 25);
	//login.Show();

	//MainMenuForm mmf(0, 0, 80, 25);
	//mmf.Show();


	JForm* form = Singleton<FormManager>::Instance().Get("LoginForm");
	form->Show();

	return app.Exec();	// 消息循环
}
