#pragma once

#include "SystemDefine.h"
#include <cstdint>

#define RCV_BUF 5000
#define ALLOCATE_BUFFER_SIZE 10

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
}
