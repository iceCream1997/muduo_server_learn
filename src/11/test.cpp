#include <stdio.h>

void foo(int x)
{
}

class B
{
public:
	virtual void foo()
	{
	}
};

class D : public B
{
public:
	void foo(int x)
	{
	}
};

template<typename To, typename From>
inline To implicit_cast(From const &f) {
  return f;
}

int main(void)
{
	int n;
	double d = 1.23;
	n = d;

	B* pb;
	D* pd = NULL;

	pb = pd;
	pb = implicit_cast<B*, D*>(pd);
	return 0;
}
