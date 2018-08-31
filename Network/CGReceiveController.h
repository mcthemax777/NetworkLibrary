#pragma once

#include "Network.h"

namespace CG
{
	class PacketFunction
	{
	public:
		PacketFunction(NetworkPacket* _packet, std::function<void(HostId, NetworkPacket*)> _packetFunction)
		{
			packet = _packet;
			packetFunction = _packetFunction;
		}

		NetworkPacket* packet;
		std::function<void(HostId, NetworkPacket*)> packetFunction;
	};

	class CGReceiveController
	{
	public:

		int CGReceiveController::processData(ConnectorInfo* connectorInfo, char* data, int dataSize);

		template<typename T, typename std::enable_if<std::is_base_of<CG::NetworkPacket, T>::value>::type* = nullptr>
		void registerPacket(std::function<void(HostId, NetworkPacket*)> onReceiveNPacket)
		{
			T* t = new T();
			NetworkPacket* packet = (NetworkPacket*)t;

			PacketFunction* pf = new PacketFunction(packet, onReceiveNPacket);

			if (npMap.insert(std::pair<npType_t, PacketFunction*>(packet->header.npType, pf)).second == false)
			{
				ErrorLog("???");
			}
		}

		void sendPacket(HostId hostId, NetworkPacket* packet);

		std::map<npType_t, PacketFunction*> npMap;
		std::map<npType_t, PacketFunction*>::iterator itr;

		friend class Network;
		friend class ClientNetwork;
	};
}