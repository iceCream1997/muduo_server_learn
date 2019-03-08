#ifndef _JTHREAD_H_
#define _JTHREAD_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace PUBLIC
{

class JThread
{
public:
	JThread(bool isAutoDel = false);
	virtual ~JThread();
	bool Start();
	virtual void Run() = 0;
	static unsigned __stdcall ThreadFun(void *p);
	
	void SetAutoDel(bool flag);
private:
	HANDLE hThread_;
	unsigned threadId_;
protected:
	void Wait(DWORD timeout=INFINITE);
	bool isAutoDel_;
};

}


#endif // _JTHREAD_H_
