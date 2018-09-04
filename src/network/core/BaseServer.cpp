#include "BaseServer.h"
#include "Network.h"

namespace CG
{
	void BaseServer::sendData(HostId hostId, char* data, int dataSize)
	{
		Network::GetInstance()->sendData(hostId, data, dataSize);
	}
}