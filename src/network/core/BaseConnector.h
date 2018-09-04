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
	};

	class BaseConnector
	{
	public:
		void start(ConnectConfig* config);
		void stop();

		std::function<void(HostId)> onConnect;
		std::function<void(HostId)> onDisconnect;
		
		virtual int processData(ConnectorInfo* connectorInfo, char* data, int dataSize) = 0;

//		DataConvertor* dataConvertor;
		//std::function<int(char*, int)> onReceiveConvertor;

		ConnectorType getConnectorType() { return type; }

		friend class Network;
		friend class ClientNetwork;
	protected:
		BaseConnector();

		ConnectorType type;
		ConnectorInfo* connectorInfo;
		bool isConnected;
	};
}
