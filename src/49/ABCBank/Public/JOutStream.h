#ifndef _JOUT_STREAM_H_
#define _JOUT_STREAM_H_

#include "JUtil.h"
#include <string>
#include <vector>
#include <algorithm>

namespace PUBLIC
{

class JOutStream
{
public:
	static const size_t kInitialSize;

	JOutStream();

	JOutStream& operator<<(uint8 x);
	JOutStream& operator<<(uint16 x);
	JOutStream& operator<<(uint32 x);

	JOutStream& operator<<(int8 x);
	JOutStream& operator<<(int16 x);
	JOutStream& operator<<(int32 x);

	JOutStream& operator<<(const std::string& str);
	
	void WriteBytes(const void* data, size_t len);
	

	void Reposition(size_t pos)
	{
		currIndex_ = pos;
	}

	void Skip(size_t len)
	{
		EnsureWritableBytes(len);
		currIndex_ += len;
	}

	void Clear()
	{
		currIndex_ = 0;
	}
	
	char* Data()
	{
		return &*buffer_.begin();
	}

	const char* Data() const
	{
		return &*buffer_.begin();
	}

	size_t Length()
	{
		return currIndex_;
	}

private:
	size_t WriteableBytes() const
	{
		return buffer_.size() - currIndex_;
	}

	void EnsureWritableBytes(size_t len)
	{
		if (WriteableBytes() < len)
			buffer_.resize(currIndex_ + len);
	}
	
	void Append(const char*  data, size_t len);
	void Append(const void*  data, size_t len);
	std::vector<char> buffer_;
	size_t currIndex_;
};

}
#endif /* _JOUT_STREAM_H_ */