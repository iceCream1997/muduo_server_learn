#include <iostream>

using namespace std;

struct Foo   
{   
	Foo() { cout << "Foo ctor" << endl; }
	Foo(const Foo&) { cout << "Foo copy ctor" << endl; }
	void operator=(const Foo&) { cout << "Foo operator=" << endl; }
	~Foo() { cout << "Foo dtor" << endl; }
};  

Foo make_foo()   
{   
	Foo f;
	return f;
	//return Foo();  
}
  
int main(void)
{
	make_foo();
	return 0;
}
