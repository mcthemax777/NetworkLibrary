#include "BaseConnector.h"
#include "Network.h"
#include "Log/Log.h"
#include "Buffer.h"
#include "ConnectorInfo.h"

#include <cstring>

namespace CG
{
	BaseConnector::BaseConnector()
	{

	}

	void BaseConnector::start(ConnectConfig* config)
	{
		//set config
		connectorInfo = new ConnectorInfo();
		connectorInfo->ip = new char[15];
		memset(connectorInfo->ip, 0, 15);
		memcpy(connectorInfo->ip, "127.0.0.1", 9);
		connectorInfo->port = config->port;

		connectorInfo->connector = this;

		//set connect in network (server : listen, client : connect with other)
		if (Network::GetInstance()->addConnector(this) == false)
		{
			ErrorLog("fucking addConnector");
			return;
		}
		
		isConnected = true;
	
	}

	void BaseConnector::stop()
	{
		//todo stop

		isConnected = false;
	}
}



