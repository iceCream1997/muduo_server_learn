#include "Deposit.h"

#include "../DAL/BankService.h"

#include "../../Public/Logging.h"
#include "../../Public/JUtil.h"
#include "../../Public/JInStream.h"
#include "../../Public/JOutStream.h"

using namespace PUBLIC;
using namespace CMD;
using namespace DAL;

void Deposit::Execute(BankSession& session)
{
	JInStream jis(session.GetRequestPack()->buf, session.GetRequestPack()->head.len);
	uint16 cmd = session.GetCmd();

	// 帐号id
	char account_id[7] = {0};
	jis.ReadBytes(account_id, 6);

	// 存款金额
	string money;
	jis>>money;

	// 以下为存款操作
	BankService dao;
	int16 error_code = 0;
	char error_msg[31] = {0};

	Account acc;
	acc.account_id = Convert::StringToInt(account_id);
	acc.balance = Convert::StringToDouble(money);

	int ret = dao.Deposit(acc);
	if (ret == 0)
	{
		LOG_INFO<<"存款成功";
	}
	else if (ret == 2)
	{
		error_code = 2;
		strcpy_s(error_msg, "帐户不存在");
		LOG_INFO<<error_msg;
	}
	else if (ret == -1)
	{
		error_code = -1;
		strcpy_s(error_msg, "数据库错误");
	}

	JOutStream jos;

	// 包头cmd+len+cnt+seq+error_code+error_msg
	jos<<cmd;
	size_t lengthPos = jos.Length();	// len位置
	jos.Skip(2);						// 为len预留两个字节
	uint16 cnt = 0;
	uint16 seq = 0;
	jos<<cnt<<seq<<error_code;
	jos.WriteBytes(error_msg, 30);

	// 包体name+balance+date
	if (error_code == 0)
	{
		string balance = Convert::DoubleToString(acc.balance);
		jos<<acc.name<<balance;
		jos.WriteBytes(acc.op_date.c_str(), 19);
	}

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

