#pragma once

#include "network/core/BaseClient.h"

namespace CG
{
	class NetworkHandler;

	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief basic tcp client module
	*/
	class Client : public BaseClient
	{
	public:
		Client();

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
		* @param const char* data : sending data
		* @param int dataSize : sending data size
		*/
		void sendMessage(const char* data, int dataSize);

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief use to send message
		* @param std::string message : sending string
		*/
		void sendMessage(std::string message);
	};
}
