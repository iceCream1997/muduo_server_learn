#include "Socket.h"
#include <assert.h>

using namespace PUBLIC;

Socket::Socket() : socket_(INVALID_SOCKET)
{

}

Socket::~Socket()
{
	if (INVALID_SOCKET != socket_)
	{
		Close();
	}
}

void Socket::Create(bool tcp/* = true*/)
{
	if (tcp)
		socket_ = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	else
		socket_ = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	assert(INVALID_SOCKET != socket_);

	int optval = 1;
	setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR,
		(const char*)&optval, sizeof optval);
}

bool Socket::IsValid() const
{
	return socket_ != INVALID_SOCKET;
}

bool Socket::Startup()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		return false;
	}


	if ( LOBYTE( wsaData.wVersion ) != 2 ||
		HIBYTE( wsaData.wVersion ) != 2 ) {
			WSACleanup( );
			return false; 
	}

	return true;

}

void Socket::Cleanup()
{
	WSACleanup();
}

bool Socket::Bind(const char* ip, unsigned short port)
{
	sockaddr_in addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	if (ip == NULL)
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		addr.sin_addr.s_addr = inet_addr(ip);


	if (SOCKET_ERROR == bind(socket_, (sockaddr*)&addr, sizeof(addr)) )
		return false;

	return true;
}

bool Socket::Listen(int backlog)
{
	if (SOCKET_ERROR == listen(socket_, backlog))
	{
		return false;
	}
	return true;
}

SOCKET Socket::Accept()
{
	sockaddr_in peerAddr = {0};
	int addrSize = sizeof(peerAddr);
	SOCKET sock = accept(socket_, (sockaddr*)&peerAddr, &addrSize);

	return sock;
}

bool Socket::Connect(const char* ip, unsigned short port)
{
	sockaddr_in addr = {0};
	addr.sin_family = AF_INET;	
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);

	if (SOCKET_ERROR == connect(socket_, (sockaddr*)&addr, sizeof(addr)) )
	{
		return false;
	}	
	return true;
}

void Socket::Close()
{
	closesocket(socket_);
	socket_ = INVALID_SOCKET;
}

int Socket::Send(const char* buf, size_t len)
{
	return send(socket_, buf, len, 0);
}

int Socket::Recv(char* buf, size_t len)
{
	return recv(socket_, buf, len, 0);
}

int Socket::SendN(const char* buf, size_t len)
{
	int nLeft;
	int nWritten;
	char* p = (char*)buf;
	nLeft = len;

	while (nLeft > 0)
	{
		nWritten = send(socket_, p, nLeft, 0);
		if (nWritten == SOCKET_ERROR)
			return nWritten;

		nLeft -= nWritten;
		p += nWritten;
	}

	return len - nLeft;
}

int Socket::RecvN(char* buf, size_t len)
{
	int nLeft;
	int nRead;
	char* p = buf;
	nLeft = len;

	while (nLeft > 0)
	{
		nRead = recv(socket_, p, nLeft, 0);
		if (nRead == SOCKET_ERROR)
		{
			return nRead;
		}
		else if (nRead == 0)
			return nRead;

		nLeft -= nRead;
		p += nRead;
	}

	return len - nLeft;
}