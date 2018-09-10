#pragma once

#include "network/core/BaseClient.h"

namespace CG
{
	class CGNetworkHandler;
	class NetworkPacket;

	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief packet client module
	*/
	class CGClient : public BaseClient
	{
	public:
		CGClient();

		/// handle receive and send
		CGNetworkHandler* networkHandler;

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief regist function that developer created
		* @param std::function<void(HostId, NetworkPacket*)> onReceiveNPacket : set extend NetworkPacket and function
		*/
		template<typename T, typename std::enable_if<std::is_base_of<CG::NetworkPacket, T>::value>::type* = nullptr>
		void registerPacket(std::function<void(HostId, NetworkPacket*)> onReceive)
		{
			//delegate to networkHandler 
			networkHandler->registerPacket<T>(onReceive);
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
		* @param NetworkPacket* packet : sending packet
		*/
		void sendPacket(NetworkPacket* packet);
	};
}