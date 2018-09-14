#include "NetworkHandler.h"
#include "network/core/Network.h"
#include "network/core/NetworkDefine.h"
#include "network/core/ConnectorInfo.h"

namespace CG
{
	NetworkHandler::NetworkHandler(std::function<void(HostId, char*, int)>* _onReceive)
	{
		onReceive = _onReceive;
	}

	int NetworkHandler::processData(ConnectorInfo* connectorInfo, char* data, int dataSize)
	{
		//no converting. just toss
		(*onReceive)(connectorInfo->getHostId(), data, dataSize);

		return dataSize;
	}

	void NetworkHandler::sendMessage(HostId hostId, const char* data, int dataSize)
	{
		//send data
		Network::GetInstance()->sendData(hostId, data, dataSize);
	}
}



