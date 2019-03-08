#include "Deposit.h"

#include "../../Public/Logging.h"
#include "../../Public/JUtil.h"

using namespace PUBLIC;
using namespace CMD;

void Deposit::Execute(BankSession& session)
{
	JOutStream jos;
	// 包头命令
	uint16 cmd = CMD_DEPOSIT;
	jos<<cmd;
	// 预留两个字节包头len(包体+包尾长度)
	size_t lengthPos = jos.Length();
	jos.Skip(2);
	// 帐号
	string account_id = session.GetAttribute("account_id");
	jos.WriteBytes(account_id.c_str(), 6);
	// 金额
	string money = session.GetAttribute("money");
	jos<<money;
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
		string name;
		jis>>name;
		jis>>money;
		char trans_date[20] = {0};
		jis.ReadBytes(trans_date, 19);
		session.SetResponse("name", name);
		session.SetResponse("balance", money);
		session.SetResponse("trans_date", trans_date);
	}
}

