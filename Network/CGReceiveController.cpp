#include "CGReceiveController.h"
#include "Network.h"
#include "NetworkPacket.h"

namespace CG
{


	int CGReceiveController::processData(ConnectorInfo* connectorInfo, char* data, int dataSize)
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

		if (*npType <= MESSAGE_TYPE_MESSAGE)
		{
			if (pDataSize >= *npSize)
			{
				//패킷 번호에 맞는 functional 함수 호출

				itr = npMap.find(*npType);

				if (itr == npMap.end())
				{
					ErrorLog("npType error %d\n", *npType);

					return -1;
				}

				NetworkPacket* np = itr->second->packet;

				auto newPacket = np->create();

				newPacket->deserialize(data);

				((itr->second->packetFunction))(connectorInfo->getHostId(), newPacket);

				return sizeof(npType_t) + sizeof(npSize_t) + *npSize;
			}
			else //if (pDataSize < *npSize)
			{
				DebugLog("data do not come complete");
				return 0;
			}
		}
		else
		{
			ErrorLog("npType is not correct");
			return -1;
		}

		return dataSize;
	}

	void CGReceiveController::sendPacket(HostId hostId, NetworkPacket* packet)
	{
		int bufferSize = packet->size();
		char* buffer = new char[bufferSize];
		packet->serialize(buffer);

		Network::GetInstance()->sendData(hostId, buffer, bufferSize);

		delete buffer;
	}
}
