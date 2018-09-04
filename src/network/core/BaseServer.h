#pragma once

#include "BaseConnector.h"

namespace CG
{
	class ServerConfig : public ConnectConfig
	{
	};

	class BaseServer : public BaseConnector
	{
	public:
		BaseServer() { type = CONNECTOR_TYPE::CONNECTOR_TYPE_SERVER; }

		void sendData(HostId hostid, char* data, int dataSize);

		friend class Network;
		friend class ClientNetwork;

	protected:
		std::map<HostId, ConnectorInfo*> connectorInfoMap;

	};
}