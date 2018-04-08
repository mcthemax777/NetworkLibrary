#include "ConnectorInfo.h"
#include "Network.h"

namespace CG
{
	void ConnectorInfo::init(Connector* _connector, HostId _hostId)
	{
		connector = _connector;
		hostId = _hostId;
		dataConvertor = connector->dataConvertor;

		buffer = nullptr;
	}

	void ConnectorInfo::sendMessage(const char* data, int dataSize)
	{
		Network::GetInstance()->sendMessage(this, data, dataSize);
	}

	void ConnectorInfo::reset()
	{
		hostId = 0;

		dataConvertor = nullptr;
		connector = nullptr;
		buffer = nullptr;
	}
}



