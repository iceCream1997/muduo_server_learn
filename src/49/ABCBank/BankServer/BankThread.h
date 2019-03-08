#ifndef _BANK_THREAD_H_
#define _BANK_THREAD_H_

#include "../Public/JThread.h"
#include "../Public/Socket.h"

#include <memory>

using namespace PUBLIC;

class BankThread : public JThread
{
public:
	BankThread(std::auto_ptr<Socket>& socket);
	~BankThread();

	void Run();

	std::auto_ptr<Socket> socket_;
};

#endif // _BANK_THREAD_H_