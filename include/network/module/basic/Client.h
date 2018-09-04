#pragma once

#include "network/core/BaseClient.h"
#include "NetworkHandler.h"

namespace CG
{
	class Client : public BaseClient
	{
	public:
		Client();

		std::function<void(HostId, char*, int)> onReceive;

		NetworkHandler* networkHandler;

		int processData(ConnectorInfo* connectorInfo, char* data, int dataSize);

		void sendMessage(const char* data, int dataSize);

		void sendMessage(std::string message);
	};
}
