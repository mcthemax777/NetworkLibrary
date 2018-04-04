#include "Peer.h"
#include "Network.h"
#include "Log/Log.h"

namespace CG
{
	void Peer::sendMessage(const char* data, int dataSize)
	{
		if (connectorInfo != nullptr)
			Network::GetInstance()->sendMessage(connectorInfo->getHostId(), data, dataSize);
		else
			ErrorLog("clientInfo is null");
	}
}
