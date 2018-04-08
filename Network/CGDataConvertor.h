#pragma once

#include "DataConvertor.h"

#include <cstdint>

namespace CG
{
	typedef int16_t messageType_t;
	typedef int32_t messageSize_t;

	class Header
	{
	public:
		messageType_t messageType;
		messageSize_t messageSize;
	};

	enum MESSAGE_TYPE
	{
		MESSAGE_TYPE_USER = 0
	};

	class CGDataConvertor : public DataConvertor
	{
	public:
		int receiveConvert(char* data, int dataSize);
		Buffer sendConvert(const char* data, int dataSize);
	};
}
