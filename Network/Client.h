#pragma once

#include "Connector.h"

namespace NetworkFramework
{
	class ClientConfig : public ConnectConfig
	{
	};

	class Client : public Connector
	{
	public:
		Client() { type = CONNECTOR_TYPE::CONNECTOR_TYPE_CLIENT; }
		void sendMessage(const char* data, int dataSize);

	};
}
