#include "CGClient.h"
#include "Network.h"

namespace CG
{
	CGClient::CGClient()
	{
		receiveHandler = new CGReceiveController();
	}

	int CGClient::processData(ConnectorInfo* connectorInfo, char* data, int dataSize)
	{
		return receiveHandler->processData(connectorInfo, data, dataSize);
	}

	void CGClient::sendPacket(NetworkPacket* packet)
	{
		receiveHandler->sendPacket(connectorInfo->getHostId(), packet);
	}
}
