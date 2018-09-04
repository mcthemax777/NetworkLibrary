#include "CGClient.h"
#include "network/core/Network.h"
#include "NetworkPacket.h"

namespace CG
{
	CGClient::CGClient()
	{
		networkHandler = new CGNetworkHandler();
	}

	int CGClient::processData(ConnectorInfo* connectorInfo, char* data, int dataSize)
	{
		return networkHandler->processData(connectorInfo, data, dataSize);
	}

	void CGClient::sendPacket(NetworkPacket* packet)
	{
		networkHandler->sendPacket(connectorInfo->getHostId(), packet);
	}
}
