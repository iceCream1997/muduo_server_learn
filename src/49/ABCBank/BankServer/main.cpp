#include "Server.h"
#include "../Public/MD5.h"

#include "DAL/MysqlDB.h"
#include "../Public/Exception.h"
#include "../Public/Logging.h"

#include <stdio.h>
#include <iostream>
using namespace std;

using namespace PUBLIC;
using namespace DAL;

int main(void)
{
	//MD5 md5;

	//unsigned char hash[16];
	//md5.MD5Make(hash, (unsigned char const *)"cppcourse", 9);
	//int i;
	//for (i=0; i<16; ++i)
	//{
	//	printf("%0X", hash[i]);
	//}

	//printf("\n");

	/*
	MysqlDB db;
	try
	{
		db.Open("localhost", "root", "123456", "scott", 0);
		unsigned long long ret = db.ExecSQL(
			"insert into emp values(8888, 'YYYY', 'CLERK', 7782, '1982-01-23', 1300, null, 30)");
		cout<<ret<<"ÐÐÊÜÓ°Ïì"<<endl;

		MysqlRecordset rs = db.QuerySQL("select * from emp where deptno=30;");
		cout<<"empno ename"<<endl;
		for (unsigned int i=0; i<rs.GetRows(); ++i)
		{
			cout<<rs.GetItem(i, "empno")<<" "<<rs.GetItem(i, "ename")<<endl;
		}
	}
	catch (Exception& e)
	{
		LOG_INFO<<e.what();
		db.Close();
	}

	db.Close();
	*/
	return Singleton<Server>::Instance().Start();
}