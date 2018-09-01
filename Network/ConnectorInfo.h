#pragma once

#include <functional>
#include <list>
#include "Define.h"

namespace CG
{
	class Buffer;
	class BaseConnector;
	class DataConvertor;

	class ConnectorInfo
	{
	public:
		HostId getHostId() { return hostId; }

		void sendData(const char* data, int dataSize);

		friend class BaseConnector;
		friend class Network;
		friend class ClientNetwork;
		friend class WorkerThread;

	protected:
		void init(BaseConnector* _connector, HostId _hostId);
		void reset();


		std::list<Buffer*> bufferList;

		char* ip;
		unsigned short port;
		HostId hostId;

		BaseConnector* connector;
	};
}