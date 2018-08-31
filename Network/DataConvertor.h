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

	class DataConvertor
	{
	public:
		virtual int receiveConvert(char* data, int dataSize) = 0;
		virtual Buffer sendConvert(const char* data, int dataSize) = 0;
	};

	class DefaultDataConvertor : public DataConvertor
	{
	public:
		int receiveConvert(char* data, int dataSize)
		{
			return dataSize;
		}

		Buffer sendConvert(const char* data, int dataSize)
		{
			Buffer buffer;
			memcpy(buffer.data, data, dataSize);
			buffer.dataSize = dataSize;

			return buffer;
		}
	};
}