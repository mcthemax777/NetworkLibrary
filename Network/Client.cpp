#include "Client.h"
#include "Network.h"
#include "Log/Log.h"
#include "WorkerThread.h"

namespace CG
{
	Client::Client()
	{
		receiveHandler = new ReceiveHandler(&onReceive);
	}

	int Client::processData(ConnectorInfo* connectorInfo, char* data, int dataSize)
	{
		return receiveHandler->processData(connectorInfo, data, dataSize);
	}

	void Client::sendMessage(const char* data, int dataSize)
	{
		Network::GetInstance()->sendData(this, data, dataSize);
	}
}
