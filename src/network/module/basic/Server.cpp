#include "Server.h"
#include "NetworkHandler.h"

namespace CG
{
	Server::Server()
	{
		//create handler
		networkHandler = new NetworkHandler(onReceive);
	}

	int Server::processData(ConnectorInfo* connectorInfo, char* data, int dataSize)
	{
		//delegate to networkHandler 
		return networkHandler->processData(connectorInfo, data, dataSize);
	}

	void Server::sendMessage(HostId hostId, char* data, int dataSize)
	{
		//delegate to networkHandler 
		networkHandler->sendMessage(connectorInfo->getHostId(), data, dataSize);
	}

	void Server::sendMessage(HostId hostId, std::string message)
	{
		//delegate to networkHandler 
		networkHandler->sendMessage(connectorInfo->getHostId(), message.c_str(), message.size());
	}
}