#include "CGServer.h"
#include "CGNetworkHandler.h"
#include "NetworkPacket.h"

namespace CG
{
	CGServer::CGServer()
	{
		//create handler
		networkHandler = new CGNetworkHandler();
	}

	int CGServer::processData(ConnectorInfo* connectorInfo, char* data, int dataSize)
	{
		//delegate to networkHandler
		return networkHandler->processData(connectorInfo, data, dataSize);
	}

	void CGServer::sendPacket(HostId hostId, NetworkPacket* packet)
	{
		//delegate to networkHandler
		networkHandler->sendPacket(hostId, packet);
	}
}
