#ifndef _THREAD_H_
#define _THREAD_H_

#include <pthread.h>

class Thread
{
public:
	Thread();
	virtual ~Thread();

	void Start();
	void Join();

	void SetAutoDelete(bool autoDelete);

private:
	static void* ThreadRoutine(void* arg);
	virtual void Run() = 0;
	pthread_t threadId_;
	bool autoDelete_;
};

#endif // _THREAD_H_
