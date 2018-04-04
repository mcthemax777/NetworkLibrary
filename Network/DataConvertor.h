#pragma once

namespace CG
{
	class DataConvertor
	{
	public:
		virtual int convert(char* data, int dataSize) = 0;
	};

	class DefaultDataConvertor : public DataConvertor
	{
	public:
		int convert(char* data, int dataSize)
		{
			return dataSize - 1;
		}
	};
}