#include "NetworkHandler.h"
#include "Network.h"
#include "Log/Log.h"
#include "Buffer.h"
#include "ConnectorInfo.h"

#include <cstring>

namespace CG
{
	NetworkHandler::NetworkHandler(std::function<void(HostId, char*, int)>* _onReceive)
	{
		onReceive = _onReceive;
	}

	int NetworkHandler::processData(ConnectorInfo* connectorInfo, char* data, int dataSize)
	{
		(*onReceive)(connectorInfo->getHostId(), data, dataSize);

		return dataSize;
	}

	void NetworkHandler::sendMessage(HostId hostId, const char* data, int dataSize)
	{
		Network::GetInstance()->sendData(hostId, data, dataSize);
	}
}



