#include "CGDataConvertor.h"
#include "Log/Log.h"

namespace CG
{
	int CGDataConvertor::receiveConvert(char* data, int dataSize)
	{
		char* pData = data;
		int pDataSize = dataSize;
		int recvCnt = dataSize;

		if (pDataSize < sizeof(npType_t))
		{
			return 0;
		}

		npType_t* npType = (npType_t*)pData;

		if (*npType < 0)
		{
			ErrorLog("npType is not correct");
			return -1;
		}

		pData += sizeof(npType_t);
		pDataSize -= sizeof(npType_t);


		if (pDataSize < sizeof(npSize_t))
		{
			return 0;
		}

		npSize_t* npSize = (npSize_t*)pData;

		if (*npSize < 0)
		{
			ErrorLog("dataType is not correct");
			return -1;
		}

		pData += sizeof(npSize_t);
		pDataSize -= sizeof(npSize_t);

		if (*npType < MESSAGE_TYPE_MESSAGE)
		{
			if (pDataSize == *npSize)
			{
				return dataSize;
			}
			else if (pDataSize < *npSize)
			{
				DebugLog("data do not come complete");
				return 0;
			}
			else
			{
				return sizeof(npType_t) + sizeof(npSize_t) + *npSize;
			}
		}
		else
		{
			ErrorLog("npType is not correct");
			return -1;
		}
	}

	Buffer CGDataConvertor::sendConvert(const char* data, int dataSize)
	{
		Buffer buffer;

		char* pData = buffer.data;
		int pDataSize = 0;

		npSize_t npSize = dataSize;
		npType_t npType = MESSAGE_TYPE_MESSAGE;

		memcpy(pData, &npType, sizeof(npType_t));

		pData += sizeof(npType_t);
		pDataSize += sizeof(npType_t);

		memcpy(pData, &npSize, sizeof(npSize_t));

		pData += sizeof(npSize_t);
		pDataSize += sizeof(npSize_t);

		memcpy(pData, data, dataSize);

		pData += dataSize;
		pDataSize += dataSize;

		buffer.dataSize = pDataSize;

		return buffer;
	}
}
