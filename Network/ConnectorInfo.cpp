#include "ConnectorInfo.h"
#include "Network.h"

namespace NetworkFramework
{
	void ConnectorInfo::sendMessage(const char* data, int dataSize)
	{
		Network::GetInstance()->sendMessage(hostId, data, dataSize);
	}
}



