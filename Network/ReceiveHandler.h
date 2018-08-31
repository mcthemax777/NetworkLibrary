#pragma once

#include <string>
#include <functional>
#include <map>

#include "Define.h"
#include "ConnectorInfo.h"
#include "NetworkPacket.h"

namespace CG
{
	class ReceiveHandler
	{
	public:
		ReceiveHandler(std::function<void(HostId, char*, int)>* _onReceive);
		
		int processData(ConnectorInfo* connectorInfo, char* data, int dataSize);

		friend class Network;
		friend class ClientNetwork;

		std::function<void(HostId, char*, int)>* onReceive;

	};
}
