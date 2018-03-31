#include "Connector.h"
#include "Network.h"
#include "Log/Log.h"
#include "DataConvertor.h"

#include <cstring>

namespace NetworkFramework
{
	Connector::Connector()
	{

	}

	void Connector::start(ConnectConfig* config)
	{
		connectorInfo = new ConnectorInfo();
		connectorInfo->ip = new char[15];
		memset(connectorInfo->ip, 0, 15);
		memcpy(connectorInfo->ip, "127.0.0.1", 9);
		connectorInfo->port = config->port;

		if (config->dataConvertor == nullptr)
			dataConvertor = new DefaultDataConvertor();
		else
			dataConvertor = config->dataConvertor;

		connectorInfo->dataConvertor = dataConvertor;
		connectorInfo->connector = this;

		if (Network::GetInstance()->addConnector(this) == false)
		{
			ErrorLog("fucking addConnector");
		}
		else
		{
			isConnected = true;
		}
	}

	void Connector::stop()
	{
		//todo stop

		isConnected = false;
	}
}



