#include "Client.h"
#include "Network.h"
#include "Log/Log.h"
#include "WorkerThread.h"

namespace CG
{
	void Client::sendMessage(char* data, int dataSize)
	{
		if (isCGModule)
		{
			MessagePacket packet;
			packet.str.dataSize = dataSize;
			packet.str.data = data;

			int bufferSize = packet.size();
			char* buffer = new char[bufferSize];
			packet.serialize(buffer);

			Network::GetInstance()->sendMessage(this, buffer, bufferSize);

			delete buffer;
		}
		else
		{
			Network::GetInstance()->sendMessage(this, data, dataSize);
		}
	}

	void Client::processMessage()
	{
		if (Network::GetInstance()->workerThreadCount == 0)
		{
			(*(Network::GetInstance()->workerThreadArray))->run();
		}
	}
}
