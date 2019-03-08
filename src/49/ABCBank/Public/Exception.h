#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <exception>
#include <string>

namespace PUBLIC
{

class Exception : public std::exception
{
public:
	explicit Exception(const char* message)
		: message_(message)
	{
		FillStackTrace();
	}

	explicit Exception(const std::string& message)
		: message_(message)
	{
		FillStackTrace();
	}

	virtual ~Exception() throw()
	{

	}

	virtual const char* what() const throw();
	const char* StackTrace() const throw();

private:
	void FillStackTrace();

	std::string message_;
	std::string stackTrace_;
};

//
//class SocketException : public Exception
//{
//public:
//	explicit SocketException(const char* message)
//		: Exception(message)
//	{
//
//	}
//	explicit SocketException(const std::string& message)
//		: Exception(message)
//	{
//
//	}
//	virtual ~SocketException() throw()
//	{
//
//	}
//};
//
//
//class SocketInputException : public SocketException
//{
//public:
//	explicit SocketInputException(const char* message)
//		: SocketException(message)
//	{
//
//	}
//	explicit SocketInputException(const std::string& message)
//		: SocketException(message)
//	{
//
//	}
//	virtual ~SocketInputException() throw()
//	{
//
//	}
//};
//
//
//class SocketOutputException : public SocketException
//{
//public:
//	explicit SocketOutputException(const char* message)
//		: SocketException(message)
//	{
//
//	}
//	explicit SocketOutputException(const std::string& message)
//		: SocketException(message)
//	{
//
//	}
//	virtual ~SocketOutputException() throw()
//	{
//
//	}
//};
//
//class DBException : public Exception
//{
//public:
//	explicit DBException(const char* message)
//		: Exception(message)
//	{
//
//	}
//	explicit DBException(const std::string& message)
//		: Exception(message)
//	{
//
//	}
//	virtual ~DBException() throw()
//	{
//
//	}
//};

}

#endif // _EXCEPTION_H_
