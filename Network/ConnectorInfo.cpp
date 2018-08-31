#include "ConnectorInfo.h"
#include "Network.h"

namespace CG
{
	void ConnectorInfo::init(BaseConnector* _connector, HostId _hostId)
	{
		connector = _connector;
		hostId = _hostId;
		bufferList.clear();
	}

	void ConnectorInfo::sendData(const char* data, int dataSize)
	{
		Network::GetInstance()->sendData(this, data, dataSize);
	}

	void ConnectorInfo::reset()
	{
		hostId = 0;
		connector = nullptr;
		bufferList.clear();
	}
}



