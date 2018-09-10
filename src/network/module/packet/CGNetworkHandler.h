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
	* @brief process send and receive packet from client, server module
	*/
	class CGNetworkHandler
	{
	public:

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief Convert data to packet
		* @param ConnectorInfo* connectorInfo : sender info
		* @param char* data : received data
		* @param int dataSize : received data size
		* @return int result : see baseConnector(same function)
		* @todo (***)serialize -> set npSize, deserialize -> do exception when dataSize small than deserializing packet
		*/
		int processData(ConnectorInfo* connectorInfo, char* data, int dataSize);

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief Convert data to packet
		* @param std::function<void(HostId, NetworkPacket*)> onReceiveNPacket : when received packet, execute this function
		*/
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

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief use to send message
		* @param const HostId hostId : user info to send
		* @param NetworkPacket* packet : packet to send
		*/
		void sendPacket(HostId hostId, NetworkPacket* packet);

		/// storage packet function
		std::map<npType_t, PacketFunction*> npMap;
		
		/// using in functions
		std::map<npType_t, PacketFunction*>::iterator itr;

		friend class Network;
	};
}