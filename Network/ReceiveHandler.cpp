#include "ReceiveHandler.h"
#include "Network.h"
#include "Log/Log.h"
#include "DataConvertor.h"
#include "ConnectorInfo.h"

#include <cstring>

namespace CG
{
	ReceiveHandler::ReceiveHandler(std::function<void(HostId, char*, int)>* _onReceive)
	{
		onReceive = _onReceive;
	}

	int ReceiveHandler::processData(ConnectorInfo* connectorInfo, char* data, int dataSize)
	{
		(*onReceive)(connectorInfo->getHostId(), data, dataSize);

		return dataSize;
	}
}



