#include "QueryDayBill.h"

#include "../../Public/Logging.h"
#include "../../Public/JUtil.h"

using namespace PUBLIC;
using namespace CMD;

void QueryDayBill::Execute(BankSession& session)
{
	JOutStream jos;
	// 包头命令
	uint16 cmd = CMD_DAY_BILL;
	jos<<cmd;
	// 预留两个字节包头len(包体+包尾长度)
	size_t lengthPos = jos.Length();
	jos.Skip(2);
	//jos<<session.GetAttribute("page");

	stringstream ss;
	ss<<session.GetAttribute("page");
	uint32 page;
	ss>>page;
	jos<<page;

	jos.WriteBytes(session.GetAttribute("date").c_str(), 10);

	// 包头len字段
	size_t tailPos = jos.Length();
	jos.Reposition(lengthPos);
	jos<<(uint16)(tailPos + 8 - sizeof(RequestHead));

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

	session.Send(jos.Data(), jos.Length());	// 发送请求包
	session.Recv();	// 接收应答包
	JInStream jis((const char*)session.GetResponsePack(), session.GetResponsePack()->head.len+sizeof(ResponseHead));
	// 跳过cmd、len
	jis.Skip(4);
	uint16 cnt;
	uint16 seq;
	int16 error_code;
	jis>>cnt>>seq>>error_code;

	char error_msg[31];
	jis.ReadBytes(error_msg, 30);

	session.SetErrorCode(error_code);
	session.SetErrorMsg(error_msg);
	if (error_code == 0)
	{
		for (uint16 i=0; i<cnt; ++i)
		{
			jis.SetData((const char*)session.GetResponsePack(),
				session.GetResponsePack()->head.len+sizeof(ResponseHead));
			// 跳过cmd、len
			jis.Skip(4);
			uint16 cnt;
			uint16 seq;
			int16 error_code;
			jis>>cnt>>seq>>error_code;

			char error_msg[31];
			jis.ReadBytes(error_msg, 30);

			char trans_date[20] = {0};
			char account_id[7] = {0};
			char other_account_id[7] = {0};
			string money;
			string abstract_name;
			string balance;
			uint32 total;
			jis.ReadBytes(trans_date, 19);
			jis.ReadBytes(account_id, 6);
			jis.ReadBytes(other_account_id, 6);
			jis>>money>>abstract_name>>balance>>total;

			TransDetail td;
			td.trans_date = trans_date;
			td.account_id = account_id;
			td.other_account_id = other_account_id;
			td.abstract_name = abstract_name;

			ss.clear();
			ss.str("");
			ss<<money;
			ss>>td.money;
			ss.clear();
			ss.str("");
			ss<<balance;
			ss>>td.balance;

			td.total = total;

			session.AddDetail(td);

			if (seq == cnt - 1)
				return;
			session.Recv();

		}
	}
}


