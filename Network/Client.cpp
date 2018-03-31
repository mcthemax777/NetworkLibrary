#include "Client.h"
#include "Network.h"
#include "Log/Log.h"

namespace NetworkFramework
{
	void Client::sendMessage(const char* data, int dataSize)
	{
		if (connectorInfo != nullptr)
			Network::GetInstance()->sendMessage(connectorInfo->getHostId(), data, dataSize);
		else
			ErrorLog("clientInfo is null");
	}
}
