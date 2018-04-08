#include "Server.h"
#include "Network.h"

namespace CG
{
	void Server::sendMessage(HostId hostId, const char* data, int dataSize)
	{
		Network::GetInstance()->sendMessage(this, hostId, data, dataSize);
	}
}