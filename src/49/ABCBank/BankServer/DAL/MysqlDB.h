#ifndef _MYSQL_DB_H_
#define _MYSQL_DB_H_

//#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <mysql.h>

#include <vector>
#include <string>
using namespace std;

namespace DAL
{

class MysqlDB;
class MysqlRecordset
{
	friend class MysqlDB;
public:
	const string& GetItem(unsigned int nRow, unsigned int nCol) const
	{
		return rows_[nRow][nCol];
	}

	const string& GetItem(unsigned int nRow, const string& name) const
	{
		unsigned int index = GetFieldIndex(name);
		return rows_[nRow][index];
	}

	unsigned int GetRows() const
	{
		return rows_.size();
	}

	unsigned int GetCols() const
	{
		return fields_.size();
	}

	unsigned int GetFieldIndex(const std::string &name) const
	{
		unsigned int index = -1;
		for(unsigned int i = 0; i < fields_.size(); ++i)
		{
			if (fields_[i].name == name)
				index = fields_[i].index;
		}
		return index;
	}

	void Clear()
	{
		rows_.clear();
		fields_.clear();
	}

	typedef struct Field
	{
		string name;
		unsigned int index;
	} FIELD;
	typedef vector<FIELD> FIELDS;
	typedef vector<string> ROW;
private:
	vector<ROW> rows_;
	FIELDS fields_;
};

class MysqlDB
{
public:
	MysqlDB();
	~MysqlDB();
	void Open(const char* host,
		const char* user,
		const char* passwd,
		const char* db,
		unsigned int port);
	void Close();

	unsigned long long ExecSQL(const char* sql);
	MysqlRecordset QuerySQL(const char* sql);

	unsigned long long GetInsertId() const;
	void StartTransaction();
	void Commit();
	void Rollback();


private:
	MYSQL* mysql_;
};

}

#endif // _MYSQL_DB_H_
