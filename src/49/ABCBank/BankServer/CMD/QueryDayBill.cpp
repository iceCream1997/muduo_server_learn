#include "QueryDayBill.h"

#include "../DAL/BankService.h"

#include "../../Public/Logging.h"
#include "../../Public/JUtil.h"
#include "../../Public/JInStream.h"
#include "../../Public/JOutStream.h"

using namespace PUBLIC;
using namespace CMD;
using namespace DAL;

void QueryDayBill::Execute(BankSession& session)
{
	JInStream jis(session.GetRequestPack()->buf, session.GetRequestPack()->head.len);
	uint16 cmd = session.GetCmd();

	// 页号
	uint32 page;
	jis>>page;
	// 日期
	char date[11] = {0};
	jis.ReadBytes(date, 10);

	// 以下为查询操作
	BankService dao;
	int16 error_code = 0;
	char error_msg[31] = {0};

	list<TransDetail> details;
	int ret = dao.QueryDayBill(details, page, date);
	if (ret == 6)
	{
		error_code = 6;
		strcpy_s(error_msg, "没有找到数据");
		LOG_INFO<<error_msg;
	}
	else if (ret == -1)
	{
		error_code = -1;
		strcpy_s(error_msg, "数据库错误");
	}

	if (error_code == 0)
	{
		LOG_INFO<<"查询成功";
		uint16 cnt = static_cast<uint16>(details.size());
		uint16 seq = 0;
		list<TransDetail>::const_iterator it;
		for (it=details.begin(); it!=details.end(); ++it)
		{
			JOutStream jos;
			// 包头cmd+len+cnt+seq+error_code+error_msg
			jos<<cmd;
			size_t lengthPos = jos.Length();	// len位置
			jos.Skip(2);						// 为len预留两个字节
			jos<<cnt<<seq<<error_code;
			seq++;
			jos.WriteBytes(error_msg, 30);

			// 包体date+account_id+other_account_id+money+abstract_name+balance+total
			jos.WriteBytes(it->trans_date.c_str(), 19);
			jos.WriteBytes(it->account_id.c_str(), 6);
			jos.WriteBytes(it->other_account_id.c_str(), 6);
			string money = Convert::DoubleToString(it->money);
			jos<<money;
			jos<<it->abstract_name;
			string balance = Convert::DoubleToString(it->balance);
			jos<<balance;
			jos<<it->total;

			// 包头len字段
			size_t tailPos = jos.Length();
			jos.Reposition(lengthPos);
			jos<<(uint16)(tailPos + 8 - sizeof(ResponseHead));	// 包体+包尾长度

			// 包尾
			jos.Reposition(tailPos);
			unsigned char hash[16];
			MD5 md5;
			md5.MD5Make(hash, (unsigned char const*)jos.Data(), jos.Length());
			for (int i=0; i<8; ++i)
			{
				hash[i] = hash[i] ^ hash[i+8];
				hash[i] = hash[i] ^ ((cmd >> (i%2)) & 0xff);
			}
			jos.WriteBytes(hash, 8);
			session.Send(jos.Data(), jos.Length());

		}
	}
	else
	{
		JOutStream jos;
		// 包头cmd+len+cnt+seq+error_code+error_msg
		uint16 cnt = 0;
		uint16 seq = 0;
		jos<<cmd;
		size_t lengthPos = jos.Length();	// len位置
		jos.Skip(2);						// 为len预留两个字节
		jos<<cnt<<seq<<error_code;
		jos.WriteBytes(error_msg, 30);

		// 包体

		// 包头len字段
		size_t tailPos = jos.Length();
		jos.Reposition(lengthPos);
		jos<<(uint16)(tailPos + 8 - sizeof(ResponseHead));	// 包体+包尾长度

		// 包尾
		jos.Reposition(tailPos);
		unsigned char hash[16];
		MD5 md5;
		md5.MD5Make(hash, (unsigned char const*)jos.Data(), jos.Length());
		for (int i=0; i<8; ++i)
		{
			hash[i] = hash[i] ^ hash[i+8];
			hash[i] = hash[i] ^ ((cmd >> (i%2)) & 0xff);
		}
		jos.WriteBytes(hash, 8);
		session.Send(jos.Data(), jos.Length());
	}
}


