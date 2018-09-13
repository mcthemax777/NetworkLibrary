#pragma once

#include "network/core/Network.h"
#include "NetworkPacket.h"

namespace CG
{
	class BaseReceivedNPHandler
	{
	public:
		virtual int process(HostId hostId, char* data, int dataSize) = 0;
	};

	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief tie up packet and packet function to save together
	*/
	template <typename T>
	class ReceivedNPHandler : public BaseReceivedNPHandler
	{
	public:

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief set member value
		* @param NetworkPacket* _packet : received packet
		* @param std::function<void(HostId, NetworkPacket*)> _packetFunction : received packet function
		*/
		ReceivedNPHandler(std::function<void(HostId, T*)> _packetFunction)
		{
			packetFunction = _packetFunction;
		}

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief create network packet
		*/
		T* createPacket()
		{
			return new T();
		}

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief deserialize packet and call developer's function
		* @param HostId hostId : sender id
		* @param char* data : received data
		* @param int dataSize : received data size
		* @param int result : converting data size
		*/
		int process(HostId hostId, char* data, int dataSize)
		{
			T* np = new T();

			//deserialize data to packet
			int deserializeSize = np->deserialize(data, dataSize);

			if (deserializeSize != dataSize)
			{
				ErrorLog("deserialize is incorrect size");
			}

			//call packet function that is created by developer
			packetFunction(hostId, np);

			//delete packet
			delete np;

			return dataSize;
		}

		///network packet function
		std::function<void(HostId, T*)> packetFunction;
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
		void registerPacket(std::function<void(HostId, T*)> onReceiveNPacket)
		{
			T t;
			ReceivedNPHandler<T>* handler = new ReceivedNPHandler<T>(onReceiveNPacket);

			if (npMap.insert(std::pair<npType_t, ReceivedNPHandler<T>*>(t.header.npType, handler)).second == false)
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

		/// storage BaseReceivedNPHandler
		std::map<npType_t, BaseReceivedNPHandler*> npMap;
		
		/// using in functions
		std::map<npType_t, BaseReceivedNPHandler*>::iterator itr;

		friend class Network;
	};
}