#pragma once

#include <string>
#include <functional>
#include <map>

#include "network/core/Define.h"
#include "network/core/ConnectorInfo.h"

namespace CG
{
	class NetworkHandler
	{
	public:
		NetworkHandler(std::function<void(HostId, char*, int)>* _onReceive);
		
		int processData(ConnectorInfo* connectorInfo, char* data, int dataSize);

		void sendMessage(HostId hostId, const char* data, int dataSize);

		friend class Network;
		friend class ClientNetwork;

		std::function<void(HostId, char*, int)>* onReceive;

	};
}
