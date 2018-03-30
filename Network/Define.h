#pragma once

#include <cstdint>

namespace NetworkFramework
{
	typedef int32_t HostId;
	typedef int8_t ConnectorType;

	class Data
	{
	public:
		char* data;
		int dataSize;
	};

	typedef Data Message;

	const int RCV_BUF = 200 * 1024;
}