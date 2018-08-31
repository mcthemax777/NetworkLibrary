#include "CGServer.h"
#include "Network.h"

namespace CG
{
	CGServer::CGServer()
	{
		receiveHandler = new CGReceiveController();

		//onReceiveConvertor = [&](char* data, int dataSize) -> int
		//{
		//	char* pData = data;
		//	int pDataSize = dataSize;
		//	int recvCnt = dataSize;

		//	if (pDataSize < sizeof(npType_t))
		//	{
		//		return 0;
		//	}

		//	npType_t* npType = (npType_t*)pData;

		//	if (*npType < 0)
		//	{
		//		ErrorLog("npType is not correct");
		//		return -1;
		//	}

		//	pData += sizeof(npType_t);
		//	pDataSize -= sizeof(npType_t);


		//	if (pDataSize < sizeof(npSize_t))
		//	{
		//		return 0;
		//	}

		//	npSize_t* npSize = (npSize_t*)pData;

		//	if (*npSize < 0)
		//	{
		//		ErrorLog("dataType is not correct");
		//		return -1;
		//	}

		//	pData += sizeof(npSize_t);
		//	pDataSize -= sizeof(npSize_t);

		//	if (*npType < MESSAGE_TYPE_MESSAGE)
		//	{
		//		if (pDataSize == *npSize)
		//		{
		//			return dataSize;
		//		}
		//		else if (pDataSize < *npSize)
		//		{
		//			DebugLog("data do not come complete");
		//			return 0;
		//		}
		//		else
		//		{
		//			return sizeof(npType_t) + sizeof(npSize_t) + *npSize;
		//		}
		//	}
		//	else
		//	{
		//		ErrorLog("npType is not correct");
		//		return -1;
		//	}
		//};

		//onReceive = [&](CG::HostId hostId, char* data, int dataSize)
		//{
		//	//패킷으로 변경 후 그 패킷에 맞는 registerPacket에 등록된 함수 호출
		//	char* pData = data;
		//	int pDataSize = dataSize;
		//	int recvCnt = dataSize;

		//	if (pDataSize < sizeof(npType_t))
		//	{
		//		return 0;
		//	}

		//	npType_t* npType = (npType_t*)pData;

		//	if (*npType < 0)
		//	{
		//		ErrorLog("npType is not correct");
		//		return -1;
		//	}

		//	pData += sizeof(npType_t);
		//	pDataSize -= sizeof(npType_t);


		//	if (pDataSize < sizeof(npSize_t))
		//	{
		//		return 0;
		//	}

		//	npSize_t* npSize = (npSize_t*)pData;

		//	if (*npSize < 0)
		//	{
		//		ErrorLog("dataType is not correct");
		//		return -1;
		//	}

		//	pData += sizeof(npSize_t);
		//	pDataSize -= sizeof(npSize_t);

		//	itr = npMap.find(*npType);
		//	if (itr == npMap.end())
		//	{
		//		ErrorLog("npType error %d\n", *npType);

		//		return;
		//	}

		//	NetworkPacket* np = itr->second;

		//	auto newPacket = np->create();

		//	
		//	newPacket->deserialize(data);

		//	(*(np->onReceiveNPacket))(hostId, newPacket);
		//};
	}

	int CGServer::processData(ConnectorInfo* connectorInfo, char* data, int dataSize)
	{
		return receiveHandler->processData(connectorInfo, data, dataSize);
	}

	void CGServer::sendPacket(HostId hostId, NetworkPacket* packet)
	{
		receiveHandler->sendPacket(hostId, packet);
	}
}
