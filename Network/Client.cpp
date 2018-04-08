#include "Client.h"
#include "Network.h"
#include "Log/Log.h"
#include "WorkerThread.h"

namespace CG
{
	void Client::sendMessage(const char* data, int dataSize)
	{
		if (connectorInfo != nullptr)
			Network::GetInstance()->sendMessage(this, data, dataSize);
		else
			ErrorLog("clientInfo is null");
	}

	void Client::processMessage()
	{
		if (Network::GetInstance()->workerThreadCount == 0)
		{
			(*(Network::GetInstance()->workerThreadArray))->run();
		}
	}
}
