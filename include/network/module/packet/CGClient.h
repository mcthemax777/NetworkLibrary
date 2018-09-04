#pragma once

#include "network/core/BaseClient.h"
#include "CGNetworkHandler.h"

namespace CG
{
	class CGClient : public BaseClient
	{
	public:
		CGClient();

		CGNetworkHandler* networkHandler;

		template<typename T, typename std::enable_if<std::is_base_of<CG::NetworkPacket, T>::value>::type* = nullptr>
		void registerPacket(std::function<void(HostId, NetworkPacket*)> onReceiveNPacket)
		{
			networkHandler->registerPacket<T>(onReceiveNPacket);
		}

		int processData(ConnectorInfo* connectorInfo, char* data, int dataSize);

		void sendPacket(NetworkPacket* packet);
	};
}