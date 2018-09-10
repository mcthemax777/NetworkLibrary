#include "CGClient.h"
#include "network/core/Network.h"
#include "NetworkPacket.h"
#include "CGNetworkHandler.h"

namespace CG
{
	CGClient::CGClient()
	{
		//create handler
		networkHandler = new CGNetworkHandler();
	}

	int CGClient::processData(ConnectorInfo* connectorInfo, char* data, int dataSize)
	{
		//delegate to networkHandler
		return networkHandler->processData(connectorInfo, data, dataSize);
	}

	void CGClient::sendPacket(NetworkPacket* packet)
	{
		//delegate to networkHandler
		networkHandler->sendPacket(connectorInfo->getHostId(), packet);
	}
}
