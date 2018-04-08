#include "CGDataConvertor.h"
#include "Log/Log.h"

namespace CG
{
	int CGDataConvertor::receiveConvert(char* data, int dataSize)
	{
		char* pData = data;
		int pDataSize = dataSize;
		int recvCnt = dataSize;

		if (pDataSize < sizeof(messageType_t))
		{
			return 0;
		}

		messageType_t* messageType = (messageType_t*)pData;

		if (*messageType < 0)
		{
			ErrorLog("messageType is not correct");
			return -1;
		}

		pData += sizeof(messageType_t);
		pDataSize -= sizeof(messageType_t);


		if (pDataSize < sizeof(messageSize_t))
		{
			return 0;
		}

		messageSize_t* messageSize = (messageSize_t*)pData;

		if (*messageSize < 0)
		{
			ErrorLog("dataType is not correct");
			return -1;
		}

		pData += sizeof(messageSize_t);
		pDataSize -= sizeof(messageSize_t);

		if (*messageType == MESSAGE_TYPE_USER)
		{
			if (pDataSize == *messageSize)
			{
				return dataSize;
			}
			else if (pDataSize < *messageSize)
			{
				DebugLog("data do not come complete");
				return 0;
			}
			else
			{
				return sizeof(messageType_t) + sizeof(messageSize_t) + *messageSize;
			}
		}
		else
		{
			ErrorLog("messageType is not correct");
			return -1;
		}

		return -1;
	}

	Buffer CGDataConvertor::sendConvert(const char* data, int dataSize)
	{
		Buffer buffer;

		char* pData = buffer.data;
		int pDataSize = 0;

		messageSize_t messageSize = dataSize;
		messageType_t messageType = MESSAGE_TYPE_USER;

		memcpy(pData, &messageType, sizeof(messageType_t));

		pData += sizeof(messageType_t);
		pDataSize += sizeof(messageType_t);

		memcpy(pData, &messageSize, sizeof(messageSize_t));

		pData += sizeof(messageSize_t);
		pDataSize += sizeof(messageSize_t);

		memcpy(pData, data, dataSize);

		pData += dataSize;
		pDataSize += dataSize;

		buffer.dataSize = pDataSize;

		return buffer;
	}
}
