#pragma once

#include "SystemDefine.h"
#include <cstdint>

namespace CG
{
	typedef int32_t HostId;

	class Data
	{
	public:
		char* data;
		int dataSize;
	};

	typedef Data Message;

	const int RCV_BUF = 200 * 1024;
}