#include "Server.h"
#include "BankThread.h"

#include "../Public/Socket.h"
#include "../Public/Logging.h"

#include <memory>
#include <iostream>
#include <sstream>

Server::Server() : config_("server.conf")
{
	Socket::Startup();
	serverIp_ = config_.GetProperty("SERVER.SERVER_IP");
	string port = config_.GetProperty("SERVER.PORT");
	stringstream ss;
	ss<<port;
	ss>>port_;

	dbServerIp_ = config_.GetProperty("DB.IP");

	port = config_.GetProperty("DB.PORT");
	ss.clear();
	ss.str("");
	ss<<port;
	ss>>dbServerPort_;

	dbUser_ = config_.GetProperty("DB.USER");
	dbPass_ = config_.GetProperty("DB.PASS");
	dbName_ = config_.GetProperty("DB.NAME");

	ss.clear();
	ss.str("");
	string interetRate = config_.GetProperty("BANK.INTERATE");
	if (interetRate.empty())
	{
		interetRate_ = 0.0035;
	}
	else
	{
		ss<<interetRate;
		ss>>interetRate_;
	}
}

Server::~Server()
{
	Socket::Cleanup();
}

int Server::Start()
{
	LOG_INFO<<"启动服务器...";
	Socket sock;
	sock.Create();
	//if (!sock.Bind(NULL, 8888))
	if (!sock.Bind(serverIp_ == "" ? NULL : serverIp_.c_str(), port_))
		return 1;
	if (!sock.Listen())
		return 1;

	while (1)
	{
		std::auto_ptr<Socket> conn(new Socket);
		conn->socket_ = sock.Accept();
		if (!conn->IsValid())
			continue;

		BankThread* bt = new BankThread(conn);	// 所有权已经转移到了BankThread中的智能指针对象
		bt->SetAutoDel(true);
		bt->Start();

	}
	return 0;
}