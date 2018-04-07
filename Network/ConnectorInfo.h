#pragma once

#include <functional>
#include "Define.h"

namespace CG
{
	class Buffer;
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
		void init(Connector* _connector, HostId _hostId);
		void reset();

		Buffer* buffer;
		int storageDataSize;

		char* ip;
		unsigned short port;
		DataConvertor* dataConvertor;
		HostId hostId;

		Connector* connector;
	};
}