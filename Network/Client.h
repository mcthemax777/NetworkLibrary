#pragma once

#include "BaseClient.h"
#include "ReceiveHandler.h"

namespace CG
{
	class Client : public BaseClient
	{
	public:
		Client();

		std::function<void(HostId, char*, int)> onReceive;

		ReceiveHandler* receiveHandler;

		int processData(ConnectorInfo* connectorInfo, char* data, int dataSize);

		void sendMessage(const char* data, int dataSize);
	};
}
