#ifndef _MD5_H_
#define _MD5_H_

namespace PUBLIC
{

class MD5
{
	typedef unsigned int word32;
	struct MD5Context
	{
		word32 buf[4];
		word32 bits[2];
		unsigned char in[64];
	} ctx;

	void MD5Init(struct MD5Context *context);
	void MD5Update(struct MD5Context *context, unsigned char const *buf,
		unsigned len);
	void MD5Final(unsigned char digest[16], struct MD5Context *context);
	void MD5Transform(word32 buf[4], word32 const in[16]);
public:
	void MD5Make(unsigned char digest[16], unsigned char const *buf,
		unsigned len);
};

}

#endif // _MD5_H_