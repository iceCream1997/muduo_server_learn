#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_

#include <string>
using namespace std;

namespace DAL
{

// ’ ∫≈
struct Account
{
	int account_id;
	string name;
	string pass;
	string id;
	string op_date;
	double balance;
};

}

#endif /* _ACCOUNT_H_ */