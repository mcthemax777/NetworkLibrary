#pragma once

#include "network/core/BaseServer.h"


namespace CG
{
	class NetworkHandler;

	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief basic tcp server module
	*/
	class Server : public BaseServer
	{
	public:
		Server();

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief storage function that developer created
		*/
		std::function<void(HostId, char*, int)> onReceive;

		/// handle receive and send
		NetworkHandler* networkHandler;

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief implement baseConnector class
		*/
		int processData(ConnectorInfo* connectorInfo, char* data, int dataSize);

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief use to send message
		* @param HostId hostId : host info to send
		* @param const char* data : sending data
		* @param int dataSize : sending data size
		*/
		void sendMessage(HostId hostid, char* data, int dataSize);
		
		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief use to send message
		* @param HostId hostId : host info to send
		* @param const char* data : sending data
		* @param int dataSize : sending data size
		*/
		void sendMessage(HostId hostId, std::string message);

	};
}