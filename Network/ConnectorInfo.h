#pragma once

#include <functional>
#include "Define.h"

namespace NetworkFramework
{
	class Connector;
	class DataConvertor;

	class ConnectorInfo
	{
	public:
		HostId getHostId() { return hostId; }

		void sendMessage(const char* data, int dataSize);

		friend class Connector;
		friend class Network;
		friend class WorkerThread;

	protected:

		char* storageData;
		int storageDataSize;

		char* ip;
		unsigned short port;
		DataConvertor* dataConvertor;
		HostId hostId;

		Connector* connector;
	};
}