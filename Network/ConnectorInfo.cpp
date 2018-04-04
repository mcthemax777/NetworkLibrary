#include "ConnectorInfo.h"
#include "Network.h"

namespace CG
{
	void ConnectorInfo::sendMessage(const char* data, int dataSize)
	{
		Network::GetInstance()->sendMessage(hostId, data, dataSize);
	}
}



