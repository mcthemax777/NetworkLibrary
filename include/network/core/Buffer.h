#pragma once

#include "Define.h"
#include <string>

namespace CG
{
	class Buffer
	{
	public:
		char data[RCV_BUF];
		int startIndex;
		int dataSize;
		bool isFinish;

		Buffer()
		{
			startIndex = 0;
			dataSize = 0;
			isFinish = false;
		}

		~Buffer()
		{
		}

		void reset() { memcpy(data, 0, RCV_BUF); startIndex = 0; dataSize = 0; isFinish = false; }
	};
}