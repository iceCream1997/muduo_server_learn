#ifndef _THREAD_H_
#define _THREAD_H_

#include <pthread.h>
#include <boost/function.hpp>

class Thread
{
public:
	typedef boost::function<void ()> ThreadFunc;
	explicit Thread(const ThreadFunc& func);

	void Start();
	void Join();

	void SetAutoDelete(bool autoDelete);

private:
	static void* ThreadRoutine(void* arg);
	void Run();
	ThreadFunc func_;
	pthread_t threadId_;
	bool autoDelete_;
};

#endif // _THREAD_H_
