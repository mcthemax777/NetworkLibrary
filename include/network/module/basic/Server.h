#pragma once

#include "network/core/BaseServer.h"


namespace CG
{
	class NetworkHandler;

	class Server : public BaseServer
	{
	public:
		Server();

		std::function<void(HostId, char*, int)> onReceive;

		NetworkHandler* networkHandler;

		int processData(ConnectorInfo* connectorInfo, char* data, int dataSize);

		void sendMessage(HostId hostid, char* data, int dataSize);
		void sendMessage(HostId hostId, std::string message);

	};
}