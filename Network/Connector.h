#pragma once

#include <string>
#include <functional>
#include <map>

#include "Define.h"
#include "ConnectorInfo.h"

namespace CG
{
	typedef int8_t ConnectorType;

	enum CONNECTOR_TYPE
	{
		CONNECTOR_TYPE_SERVER = 0,
		CONNECTOR_TYPE_CLIENT,
		CONNECTOR_TYPE_PEER,
	};

	class ConnectConfig
	{
	public:
		std::string ip;
		unsigned short port;
		DataConvertor* dataConvertor;
	};

	class Connector
	{
	public:
		void start(ConnectConfig* config);
		void stop();

		std::function<void(HostId)> onConnect;
		std::function<void(HostId)> onDisconnect;
		std::function<void(HostId, char*, int)> onReceive;
		DataConvertor* dataConvertor;

		ConnectorType getConnectorType() { return type; }

		friend class Network;
	protected:
		Connector();

		ConnectorType type;
		ConnectorInfo* connectorInfo;
		bool isConnected;
	};
}
