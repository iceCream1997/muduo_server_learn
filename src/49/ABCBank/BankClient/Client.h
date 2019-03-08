#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "BankSession.h"
#include "../Public/SystemConfig.h"
#include "../Public/Singleton.h"


using namespace PUBLIC;

class Client
{
	friend class Singleton<Client>;
public:
	const string& GetServerIp() const
	{
		return serverIp_;
	}

	unsigned short GetPort() const
	{
		return port_;
	}

	BankSession* GetBankSession()
	{
		return bs_;
	}
private:
	Client();
	Client(const Client& rhs);
	~Client();

	SystemConfig config_;

	string serverIp_;
	unsigned short port_;

	BankSession* bs_;
};

#endif // _CLIENT_H_
