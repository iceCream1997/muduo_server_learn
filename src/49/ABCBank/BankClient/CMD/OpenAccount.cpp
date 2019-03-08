#include "OpenAccount.h"

#include "../../Public/Logging.h"
#include "../../Public/JUtil.h"

using namespace PUBLIC;
using namespace CMD;

void OpenAccount::Execute(BankSession& session)
{
	JOutStream jos;
	// 包头命令
	uint16 cmd = CMD_OPEN_ACCOUNT;
	jos<<cmd;
	// 预留两个字节包头len(包体+包尾长度)
	size_t lengthPos = jos.Length();
	jos.Skip(2);
	// 客户姓名
	string name = session.GetAttribute("name");
	jos<<name;
	// 帐号密码
	string pass = session.GetAttribute("pass");
	unsigned char ideaKey[16];
	unsigned char buf[2];
	buf[0] = (cmd >> 8) & 0xff;
	buf[1] = cmd & 0xff;
	MD5 md5;
	md5.MD5Make(ideaKey, buf, 2);
	for (int i=0; i<8; ++i)
	{
		ideaKey[i] = ideaKey[i] ^ ideaKey[i+8];
		ideaKey[i] = ideaKey[i] ^ ((cmd >> (i%2)) & 0xff);
		ideaKey[i+8] = ideaKey[i] ^ ideaKey[i+8];
		ideaKey[i+8] = ideaKey[i+8] ^ ((cmd >> (i%2)) & 0xff);
	}
	char encryptedPass[16];
	Idea idea;
	idea.Crypt(ideaKey,(const unsigned char*)pass.c_str(), (unsigned char *)encryptedPass, 16, true);	// 加密
	jos.WriteBytes(encryptedPass, 16);



	// 身份证
	string id = session.GetAttribute("id");
	jos.WriteBytes(id.c_str(), id.length());
	// 开户金额
	jos<<session.GetAttribute("money");

	// 包头len字段
	size_t tailPos = jos.Length();
	jos.Reposition(lengthPos);
	jos<<(uint16)(tailPos + 8 - sizeof(RequestHead));

	// 包尾
	jos.Reposition(tailPos);
	unsigned char hash[16];
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
		char account_id[7] = {0};
		jis.ReadBytes(account_id, 6);

		string open_date;
		jis>>open_date;

		session.SetResponse("account_id", account_id);
		session.SetResponse("open_date", open_date);
	}
}


