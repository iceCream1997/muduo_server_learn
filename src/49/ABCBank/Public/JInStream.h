#ifndef _JIN_STREAM_H_
#define _JIN_STREAM_H_

#include "JUtil.h"

#include <string>
#include <vector>

#include <assert.h>

namespace PUBLIC
{

class JInStream
{
public:
	JInStream();
	JInStream(const char* data, size_t len);

	void SetData(const char* data, size_t len);

	JInStream& operator>>(uint8& x);
	JInStream& operator>>(uint16& x);
	JInStream& operator>>(uint32& x);

	JInStream& operator>>(int8& x);
	JInStream& operator>>(int16& x);
	JInStream& operator>>(int32& x);

	JInStream& operator>>(std::string& str);
	
	void Reposition(size_t pos)
	{
		currIndex_ = pos;
	}

	void Skip(size_t len)
	{
		assert(ReadableBytes() > len);
		currIndex_ += len;
	}
	
	void ReadBytes(void* data, size_t len);

private:
	size_t ReadableBytes() const
	{
		return buffer_.size() - currIndex_;
	}

	const char* Peek() const
	{
		return &*buffer_.begin() + currIndex_;
	}

	std::vector<char> buffer_;
	size_t currIndex_;
};

}

#endif /* _JIN_STREAM_H_ */
