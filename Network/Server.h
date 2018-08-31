#pragma once

#include "BaseServer.h"


namespace CG
{
	class ReceiveHandler;

	class Server : public BaseServer
	{
	public:
		Server();

		std::function<void(HostId, char*, int)> onReceive;

		ReceiveHandler* receiveHandler;

		int processData(ConnectorInfo* connectorInfo, char* data, int dataSize);

		void sendMessage(HostId hostid, char* data, int dataSize);
	};
}