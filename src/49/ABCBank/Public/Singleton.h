#ifndef _SINGLETON_H_
#define _SINGLETON_H_

namespace PUBLIC
{

template <typename T>
class Singleton
{
public:
	static T& Instance()
	{
		static T instance;
		return instance;
	}
private:
	Singleton();
	~Singleton();
};

}

#endif	// _SINGLETON_H_
