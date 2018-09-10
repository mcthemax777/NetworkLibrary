#include "Client.h"
#include "NetworkHandler.h"


namespace CG
{
	Client::Client()
	{
		//create handler
		networkHandler = new NetworkHandler(onReceive);
	}

	int Client::processData(ConnectorInfo* connectorInfo, char* data, int dataSize)
	{
		//delegate to networkHandler 
		return networkHandler->processData(connectorInfo, data, dataSize);
	}

	void Client::sendMessage(const char* data, int dataSize)
	{
		//delegate to networkHandler 
		networkHandler->sendMessage(connectorInfo->getHostId(), data, dataSize);
	}

	void Client::sendMessage(std::string message)
	{
		//delegate to networkHandler 
		networkHandler->sendMessage(connectorInfo->getHostId(), message.c_str(), message.size());
	}
}
