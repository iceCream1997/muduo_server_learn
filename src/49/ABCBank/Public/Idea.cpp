#include "Idea.h"

namespace PUBLIC
{

static unsigned short MulInv(unsigned short x)
{
	unsigned short t0, t1;
	unsigned short q, y;

	if (x <= 1)
		return x;	
	t1 = (unsigned short)(0x10001L / x);		/* Since x >= 2, this fits into 16 bits */
	y = 0x10001L % x;
	if (y == 1)
		return low16(1 - t1);

	t0 = 1;
	do 
	{
		q = x / y;
		x = x % y;
		t0 += q * t1;
		if (x == 1)
			return t0;
		q = y / x;
		y = y % x;
		t1 += q * t0;
	} while (y != 1);

	return low16(1 - t1);
}

/*
* Expand a 128-bit user key to a working encryption key EK
*/
static void IdeaExpandKey(unsigned char const *userkey, unsigned short * EK)
{
	int i, j;

	for (j = 0; j < 8; j++) 
	{
		EK[j] = (userkey[0] << 8) + userkey[1];
		userkey += 2;
	}
	for (i = 0; j < IDEAKEYLEN; j++) 
	{
		i++;
		EK[i + 7] = EK[i & 7] << 9 | EK[i + 1 & 7] >> 7;
		EK += i & 8;
		i &= 7;
	}
}				/* ideaExpandKey */

/*
* Compute IDEA decryption key DK from an expanded IDEA encryption key EK
* Note that the input and output may be the same.  Thus, the key is
* inverted into an internal buffer, and then copied to the output.
*/
static void IdeaInvertKey(unsigned short const *EK, unsigned short DK[IDEAKEYLEN])
{
	int i;
	unsigned short t1, t2, t3;
	unsigned short temp[IDEAKEYLEN];
	unsigned short *p = temp + IDEAKEYLEN;

	t1 = MulInv(*EK++);
	t2 = -*EK++;
	t3 = -*EK++;
	*--p = MulInv(*EK++);
	*--p = t3;
	*--p = t2;
	*--p = t1;

	for (i = 0; i < IDEAROUNDS - 1; i++) 
	{
		t1 = *EK++;
		*--p = *EK++;
		*--p = t1;

		t1 = MulInv(*EK++);
		t2 = -*EK++;
		t3 = -*EK++;
		*--p = MulInv(*EK++);
		*--p = t2;
		*--p = t3;
		*--p = t1;
	}

	t1 = *EK++;
	*--p = *EK++;
	*--p = t1;

	t1 = MulInv(*EK++);
	t2 = -*EK++;
	t3 = -*EK++;
	*--p = MulInv(*EK++);
	*--p = t3;
	*--p = t2;
	*--p = t1;

	/* Copy and destroy temp copy */
	memcpy(DK, temp, sizeof(temp));
	burn(temp);
}				/* ideaInvertKey */

/*
* MUL(x,y) computes x = x*y, modulo 0x10001.  Requires two temps, 
* t16 and t32.  x is modified, and must be a side-effect-free lvalue.
* y may be anything, but unlike x, must be strictly less than 65536 
* even if low16() is #defined.
* All of these are equivalent - see which is faster on your machine
*/
#define MUL(x,y) \
	((t16 = (y)) ? \
	(x=low16(x)) ? \
	t32 = (word32)x*t16, \
	x = (unsigned short)low16(t32), \
	t16 = (unsigned short)(t32>>16), \
	x = (x-t16)+(x<t16) \
	: \
	(x = 1-t16) \
	: \
	(x = 1-x))

/* IDEA encryption/decryption algorithm */
/* Note that in and out can be the same buffer */
static void IdeaCipher(unsigned char const inbuf[8], unsigned char outbuf[8],
					   unsigned short const *key)
{
	register unsigned short x1, x2, x3, x4, s2, s3;
	unsigned short *in, *out;
	register unsigned short t16;	/* Temporaries needed by MUL macro */
	register word32 t32;
	int r = IDEAROUNDS;

	in = (unsigned short *) inbuf;
	x1 = *in++;
	x2 = *in++;
	x3 = *in++;
	x4 = *in;
	x1 = (x1 >> 8) | (x1 << 8);
	x2 = (x2 >> 8) | (x2 << 8);
	x3 = (x3 >> 8) | (x3 << 8);
	x4 = (x4 >> 8) | (x4 << 8);

	do 
	{
		MUL(x1, *key++);
		x2 += *key++;
		x3 += *key++;
		MUL(x4, *key++);

		s3 = x3;
		x3 ^= x1;
		MUL(x3, *key++);
		s2 = x2;
		x2 ^= x4;
		x2 += x3;
		MUL(x2, *key++);
		x3 += x2;

		x1 ^= x2;
		x4 ^= x3;

		x2 ^= s3;
		x3 ^= s2;
	} while (--r);

	MUL(x1, *key++);
	x3 += *key++;
	x2 += *key++;
	MUL(x4, *key);

	out = (unsigned short *) outbuf;
	x1 = low16(x1);
	x2 = low16(x2);
	x3 = low16(x3);
	x4 = low16(x4);
	*out++ = (x1 >> 8) | (x1 << 8);
	*out++ = (x3 >> 8) | (x3 << 8);
	*out++ = (x2 >> 8) | (x2 << 8);
	*out = (x4 >> 8) | (x4 << 8);
}				/* ideaCipher */

/*************************************************************************/
void Idea::IdeaCfbReinit(struct IdeaCfbContext *ctx, unsigned char const *iv)
{
	if (iv)
		memcpy(ctx->iv, iv, 8);
	else
		fill0(ctx->iv, 8);
	ctx->bufleft = 0;
}

void Idea::IdeaCfbInit(struct IdeaCfbContext *ctx, unsigned char const key[16])
{
	IdeaExpandKey(key, ctx->key);
	IdeaCfbReinit(ctx, 0);
}

void Idea::IdeaCfbDestroy(struct IdeaCfbContext *ctx)
{
	burn(*ctx);
}

void Idea::IdeaCfbSync(struct IdeaCfbContext *ctx)
{
	int bufleft = ctx->bufleft;

	if (bufleft) 
	{
		memmove(ctx->iv + bufleft, ctx->iv, 8 - bufleft);
		memcpy(ctx->iv, ctx->oldcipher + 8 - bufleft, bufleft);
		ctx->bufleft = 0;
	}
}

/*
* Encrypt a buffer of data, using IDEA in CFB mode.
* There are more compact ways of writing this, but this is
* written for speed.
*/
void Idea::IdeaCfbEncrypt(struct IdeaCfbContext *ctx,
						  unsigned char const *src, unsigned char *dest, int count)
{
	int bufleft = ctx->bufleft;
	unsigned char *bufptr = ctx->iv + 8 - bufleft;

	/* If there are no more bytes to encrypt that there are bytes
	* in the buffer, XOR them in and return.
	*/
	if (count <= bufleft) 
	{
		ctx->bufleft = bufleft - count;
		while (count--) 
		{
			*dest++ = *bufptr++ ^= *src++;
		}
		return;
	}
	count -= bufleft;
	/* Encrypt the first bufleft (0 to 7) bytes of the input by XOR
	* with the last bufleft bytes in the iv buffer.
	*/
	while (bufleft--) 
	{
		*dest++ = (*bufptr++ ^= *src++);
	}
	/* Encrypt middle blocks of the input by cranking the cipher,
	* XORing 8-unsigned char blocks, and repeating until the count
	* is 8 or less.
	*/
	while (count > 8) 
	{
		bufptr = ctx->iv;
		memcpy(ctx->oldcipher, bufptr, 8);
		IdeaCipher(bufptr, bufptr, ctx->key);
		bufleft = 8;
		count -= 8;
		do 
		{
			*dest++ = (*bufptr++ ^= *src++);
		} while (--bufleft);
	}
	/* Do the last 1 to 8 bytes */
	bufptr = ctx->iv;
	memcpy(ctx->oldcipher, bufptr, 8);
	IdeaCipher(bufptr, bufptr, ctx->key);
	ctx->bufleft = 8 - count;
	do 
	{
		*dest++ = (*bufptr++ ^= *src++);
	} while (--count);
}


/*
* Decrypt a buffer of data, using IDEA in CFB mode.
* There are more compact ways of writing this, but this is
* written for speed.
*/
void Idea::IdeaCfbDecrypt(struct IdeaCfbContext *ctx,
						  unsigned char const *src, unsigned char *dest, int count)
{
	int bufleft = ctx->bufleft;
	static unsigned char *bufptr;
	unsigned char t;

	bufptr = ctx->iv + (8 - bufleft);
	if (count <= bufleft) 
	{
		ctx->bufleft = bufleft - count;
		while (count--) 
		{
			t = *bufptr;
			*dest++ = t ^ (*bufptr++ = *src++);
		}
		return;
	}
	count -= bufleft;

	while (bufleft--) 
	{
		t = *bufptr;
		*dest++ = t ^ (*bufptr++ = *src++);
	}

	while (count > 8) 
	{
		bufptr = ctx->iv;
		memcpy(ctx->oldcipher, bufptr, 8);
		IdeaCipher(bufptr, bufptr, ctx->key);
		bufleft = 8;
		count -= 8;
		do 
		{
			t = *bufptr;
			*dest++ = t ^ (*bufptr++ = *src++);
		} while (--bufleft);
	}

	bufptr = ctx->iv;
	memcpy(ctx->oldcipher, bufptr, 8);
	IdeaCipher(bufptr, bufptr, ctx->key);
	ctx->bufleft = 8 - count;

	do 
	{
		t = *bufptr;
		*dest++ = t ^ (*bufptr++ = *src++);
	} while (--count);
}

void Idea::Crypt(unsigned char const key[16],
			unsigned char const *src, unsigned char *dest, int count,bool encrypt)
{
	IdeaCfbInit(&cfb, key);
	IdeaCfbSync(&cfb);
	if(encrypt)
	{
		IdeaCfbEncrypt(&cfb, src, dest, count);
	}
	else
	{
		IdeaCfbDecrypt(&cfb, src, dest, count);
	}
}

}