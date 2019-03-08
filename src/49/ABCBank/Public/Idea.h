#ifndef _IDEA_H_
#define _IDEA_H_

#include <string.h>

namespace PUBLIC
{

typedef unsigned int word32;
#define fill0(buffer,count)	memset(buffer, 0, count)
#define burn(x) fill0((void *)&(x),sizeof(x))
#define IDEAKEYSIZE 16
#define IDEABLOCKSIZE 8
#define IDEAROUNDS 8
#define IDEAKEYLEN (6*IDEAROUNDS+4)
#define low16(x) ((x) & 0xFFFF)


class Idea
{
	struct IdeaCfbContext
	{
		unsigned char oldcipher[8];
		unsigned char iv[8];
		unsigned short key[IDEAKEYLEN];
		int bufleft;
	} cfb;

	void IdeaCfbReinit(struct IdeaCfbContext *ctx, unsigned char const *iv);
	void IdeaCfbInit(struct IdeaCfbContext *ctx, unsigned char const key[16]);
	void IdeaCfbSync(struct IdeaCfbContext *ctx);
	void IdeaCfbDestroy(struct IdeaCfbContext *ctx);
	void IdeaCfbEncrypt(struct IdeaCfbContext *ctx,
		unsigned char const *src, unsigned char *dest, int count);
	void IdeaCfbDecrypt(struct IdeaCfbContext *ctx,
		unsigned char const *src, unsigned char *dest, int count);

public:
	void Crypt(unsigned char const key[16],
		unsigned char const *src, unsigned char *dest, int count,bool encrypt);
};

}
#endif // _IDEA_H_