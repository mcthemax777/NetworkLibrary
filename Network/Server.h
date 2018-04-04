#pragma once

#include "Connector.h"

namespace CG
{
	class ServerConfig : public ConnectConfig
	{
	};

	class Server : public Connector
	{
	public:
		Server() { type = CONNECTOR_TYPE::CONNECTOR_TYPE_SERVER; }

		void sendMessage(HostId hostid, char* data, int dataSize);

		friend class Network;

	protected:
		std::map<HostId, ConnectorInfo*> connectorInfoMap;

	};
}