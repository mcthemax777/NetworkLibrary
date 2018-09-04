#pragma once

#include "Server.h"
#include "CGNetworkHandler.h"

namespace CG
{
	class CGServer : public BaseServer
	{
	public:
		CGServer();

		CGNetworkHandler* networkHandler;


		template<typename T, typename std::enable_if<std::is_base_of<CG::NetworkPacket, T>::value>::type* = nullptr>
		void registerPacket(std::function<void(HostId, NetworkPacket*)> onReceiveNPacket)
		{
			networkHandler->registerPacket<T>(onReceiveNPacket);
		}

		int processData(ConnectorInfo* connectorInfo, char* data, int dataSize);

		void sendPacket(HostId hostId, NetworkPacket* packet);
	};
}