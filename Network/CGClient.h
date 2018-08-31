#pragma once

#include "Client.h"
#include "CGReceiveController.h"

namespace CG
{
	class CGClient : public BaseClient
	{
	public:
		CGClient();

		CGReceiveController* receiveHandler;

		template<typename T, typename std::enable_if<std::is_base_of<CG::NetworkPacket, T>::value>::type* = nullptr>
		void registerPacket(std::function<void(HostId, NetworkPacket*)> onReceiveNPacket)
		{
			receiveHandler->registerPacket<T>(onReceiveNPacket);
		}

		int processData(ConnectorInfo* connectorInfo, char* data, int dataSize);

		void sendPacket(NetworkPacket* packet);
	};
}