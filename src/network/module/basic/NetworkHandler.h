#pragma once

#include <functional>
#include "network/core/NetworkDefine.h"

namespace CG
{
	class ConnectorInfo;
	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief process send and receive from client, server module
	*/
	class NetworkHandler
	{
	public:
		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief set member value
		* @param std::function<void(HostId, char*, int)>* _onReceive : storage function that developer created
		*/
		NetworkHandler(std::function<void(HostId, char*, int)>* _onReceive);
		
		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief just toss data to onReceive()
		* @param ConnectorInfo* connectorInfo : sender info
		* @param char* data : received data
		* @param int dataSize : received data size
		* @return int result : see baseConnector(same function)
		*/
		int processData(ConnectorInfo* connectorInfo, char* data, int dataSize);

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief use to send message
		* @param const HostId hostId : user info to send
		* @param const char* data : sending data
		* @param int dataSize : sending data size
		*/
		void sendMessage(HostId hostId, const char* data, int dataSize);

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief storage function that developer created
		*/
		std::function<void(HostId, char*, int)>* onReceive;

		friend class Network;
	};
}
