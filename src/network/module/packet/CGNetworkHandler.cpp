#include "CGNetworkHandler.h"
#include "network/core/Network.h"
#include "NetworkPacket.h"

namespace CG
{
	int CGNetworkHandler::processData(ConnectorInfo* connectorInfo, char* data, int dataSize)
	{
		char* pData = data;
		int pDataSize = dataSize;
		int recvCnt = dataSize;

		//if dataSize small than packet type
		if (pDataSize < sizeof(npType_t))
		{
			return 0;
		}

		npType_t* npType = (npType_t*)pData;

		//if not correct type
		if (*npType < 0)
		{
			ErrorLog("npType is not correct");
			return -1;
		}

		pData += sizeof(npType_t);
		pDataSize -= sizeof(npType_t);

		//if dataSize small than packet size
		if (pDataSize < sizeof(npSize_t))
		{
			return 0;
		}

		npSize_t* npSize = (npSize_t*)pData;

		//if incorrect size
		if (*npSize < 0)
		{
			ErrorLog("dataType is not correct");
			return -1;
		}

		pData += sizeof(npSize_t);
		pDataSize -= sizeof(npSize_t);

		if (*npType < NETWORK_PACKET_COUNT)
		{
			if (dataSize >= *npSize)
			{
				//패킷 번호에 맞는 functional 함수 호출

				itr = npMap.find(*npType);

				if (itr == npMap.end())
				{
					ErrorLog("npType error %d\n", *npType);

					return -1;
				}

				int resultSize = itr->second->process(connectorInfo->getHostId(), data, *npSize);

				// return dataSize
				return sizeof(npType_t) + sizeof(npSize_t) + resultSize;
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
	}

	void CGNetworkHandler::sendPacket(HostId hostId, NetworkPacket* packet)
	{
		//set packet size excepted header size in npSize
		packet->setPacketSize();

		//new data
		int dataSize = packet->size();
		char* data = new char[dataSize];

		//serialize packet
		packet->serialize(data, dataSize);

		//send to network
		Network::GetInstance()->sendData(hostId, data, dataSize);

		//delete data
		delete data;
	}
}
