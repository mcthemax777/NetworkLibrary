#include "Client.h"
#include "Network.h"
#include "Log/Log.h"

namespace CG
{
	void Client::sendMessage(const char* data, int dataSize)
	{
		if (connectorInfo != nullptr)
			Network::GetInstance()->sendMessage(this, data, dataSize);
		else
			ErrorLog("clientInfo is null");
	}
}
