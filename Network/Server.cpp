#include "Server.h"
#include "Network.h"

namespace CG
{
	void Server::sendMessage(HostId hostId, char* data, int dataSize)
	{
		if (isCGModule)
		{
			MessagePacket packet;
			packet.str.dataSize = dataSize;
			packet.str.data = data;

			int bufferSize = packet.size();
			char* buffer = new char[bufferSize];
			packet.serialize(buffer);

			Network::GetInstance()->sendMessage(this, hostId, buffer, bufferSize);

			delete buffer;
		}
		else
		{
			Network::GetInstance()->sendMessage(this, hostId, data, dataSize);
		}
	}
}