#include "Server.h"
#include "Network.h"

namespace NetworkFramework
{
	void Server::sendMessage(HostId hostId, char* data, int dataSize)
	{
		Network::GetInstance()->sendMessage(connectorInfo->getHostId(), data, dataSize);
	}
}