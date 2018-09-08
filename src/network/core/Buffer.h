#pragma once

#include <string>

namespace CG
{
	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief buffer that move receive data, storage non-finished data
	*/
	class Buffer
	{
	public:
		///data in buffer
		char data[RCV_BUF];
		///data start index
		int startIndex;
		///data size
		int dataSize;

		Buffer()
		{
			reset();
		}

		~Buffer()
		{
		}

		///reset buffer
		void reset() { startIndex = 0; dataSize = 0; }
	};
}