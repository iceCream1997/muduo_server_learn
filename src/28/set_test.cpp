#include <set>
#include <iostream>

using namespace std;

int main(void)
{
	int a[] = { 1, 2, 3, 4, 5};
	set<int> s(a, a+5);

	cout<<*s.lower_bound(2)<<endl;
	cout<<*s.upper_bound(2)<<endl;
	return 0;
}
