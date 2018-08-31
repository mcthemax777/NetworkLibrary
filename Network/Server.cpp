#include "Server.h"
#include "Network.h"
#include "ReceiveHandler.h"

namespace CG
{
	Server::Server()
	{
		receiveHandler = new ReceiveHandler(&onReceive);
	}

	int Server::processData(ConnectorInfo* connectorInfo, char* data, int dataSize)
	{
		return receiveHandler->processData(connectorInfo, data, dataSize);
	}

	void Server::sendMessage(HostId hostId, char* data, int dataSize)
	{
		Network::GetInstance()->sendData(hostId, data, dataSize);
	}
}