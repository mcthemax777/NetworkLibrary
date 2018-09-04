#include "Client.h"
#include "network/core/Network.h"
#include "Log/Log.h"
#include "network/core/WorkerThread.h"

namespace CG
{
	Client::Client()
	{
		networkHandler = new NetworkHandler(&onReceive);
	}

	int Client::processData(ConnectorInfo* connectorInfo, char* data, int dataSize)
	{
		return networkHandler->processData(connectorInfo, data, dataSize);
	}

	void Client::sendMessage(const char* data, int dataSize)
	{
		networkHandler->sendMessage(connectorInfo->getHostId(), data, dataSize);
	}

	void Client::sendMessage(std::string message)
	{
		Network::GetInstance()->sendData(connectorInfo->getHostId(), message.c_str(), message.size());
	}
}
