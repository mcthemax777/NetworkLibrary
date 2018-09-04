#include "Server.h"
#include "network/core/Network.h"
#include "NetworkHandler.h"

namespace CG
{
	Server::Server()
	{
		networkHandler = new NetworkHandler(&onReceive);
	}

	int Server::processData(ConnectorInfo* connectorInfo, char* data, int dataSize)
	{
		return networkHandler->processData(connectorInfo, data, dataSize);
	}

	void Server::sendMessage(HostId hostId, char* data, int dataSize)
	{
		Network::GetInstance()->sendData(hostId, data, dataSize);
	}

	void Server::sendMessage(HostId hostId, std::string message)
	{
		Network::GetInstance()->sendData(hostId, message.c_str(), message.size());
	}
}