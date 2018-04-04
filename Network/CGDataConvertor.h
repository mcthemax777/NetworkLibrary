#pragma once

#include "DataConvertor.h"

#include <cstdint>

namespace CG
{
	typedef int16_t dataType_t;
	typedef int32_t dataSize_t;

	class Header
	{
	public:
		dataSize_t dataSize;
		dataType_t dataType;
	};

	class CGDataConvertor : public DataConvertor
	{
	public:
		int convert(char* data, int dataSize);
	};
}
