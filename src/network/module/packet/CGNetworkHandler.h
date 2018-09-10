#pragma once

#include "network/core/Network.h"
#include "NetworkPacket.h"

namespace CG
{
	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief tie up packet and packet function to save together
	*/
	class PacketFunction
	{
	public:

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief set member value
		* @param NetworkPacket* _packet : received packet
		* @param std::function<void(HostId, NetworkPacket*)> _packetFunction : received packet function
		*/
		PacketFunction(NetworkPacket* _packet, std::function<void(HostId, NetworkPacket*)> _packetFunction)
		{
			packet = _packet;
			packetFunction = _packetFunction;
		}

		///network packet
		NetworkPacket* packet;

		///network packet function
		std::function<void(HostId, NetworkPacket*)> packetFunction;
	};

	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief tie up packet and packet function to save together
	*/
	class CGNetworkHandler
	{
	public:

		int processData(ConnectorInfo* connectorInfo, char* data, int dataSize);

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
	};
}