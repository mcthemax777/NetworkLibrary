#pragma once

#include "DataConvertor.h"
#include "NetworkPacket.h"
#include <cstdint>

namespace CG
{
	class CGDataConvertor : public DataConvertor
	{
	public:
		int receiveConvert(char* data, int dataSize);
		Buffer sendConvert(const char* data, int dataSize);
	};
}
