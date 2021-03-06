#pragma once

#include "network/core/BaseServer.h"
#include "CGNetworkHandler.h"
#include "NetworkPacket.h"

namespace CG
{
	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief packet server module
	*/
	class CGServer : public BaseServer
	{
	public:
		CGServer();

		/// handle receive and send
		CGNetworkHandler* networkHandler;

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief regist function that developer created
		* @param std::function<void(HostId, T*)> onReceiveNPacket : set extend T(extends NetworkPacket) and function
		*/
		template<typename T, typename std::enable_if<std::is_base_of<CG::NetworkPacket, T>::value>::type* = nullptr>
		void registerPacket(std::function<void(HostId, T*)> onReceiveNPacket)
		{
			networkHandler->registerPacket<T>(onReceiveNPacket);
		}

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief implement baseConnector class
		*/
		int processData(ConnectorInfo* connectorInfo, char* data, int dataSize);

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief use to send packet
		* @param HostId hostId : host info to send
		* @param NetworkPacket* packet : sending packet
		*/
		void sendPacket(HostId hostId, NetworkPacket* packet);
	};

}