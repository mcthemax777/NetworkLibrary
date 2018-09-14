#pragma once

#include <string>
#include <functional>
#include <map>

#include "ConnectorInfo.h"

namespace CG
{
	typedef int8_t ConnectorType;

	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief connector type that regist to connector
	* @todo none
	*/
	enum CONNECTOR_TYPE
	{
		CONNECTOR_TYPE_SERVER = 0,
		CONNECTOR_TYPE_CLIENT,
		CONNECTOR_TYPE_PEER, ///< P2P connector
	};

	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief Set ip, port to connect with other
	* @todo none
	*/
	class ConnectConfig
	{
	public:
		std::string ip;
		unsigned short port;
	};

	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief This is a base class to make child class that can use developers
	*/
	class BaseConnector
	{
	public:
		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief This is a base class to make child class that can use developers
		* @param ConnectConfig set ip, port
		*/
		void start(ConnectConfig* config);

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief stop all action (listen, connect... etc)
		* @todo clean memory
		*/
		void stop();

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief execute function when came requesting connection
		*/
		std::function<void(HostId)> onConnect;

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief execute function when came requesting disconnection
		*/
		std::function<void(HostId)> onDisconnect;
		
		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief network call this function when came receiving data from others
		* @param ConnectorInfo* connectorInfo : infomation who send data
		* @param char* data : receive data from other
		* @param int dataSize : receive data size
		* @return int index : if data doesn't come all, return 0. if data is not correct, return nagative number. if data come perfect, return data size
		*/
		virtual int processData(ConnectorInfo* connectorInfo, char* data, int dataSize) = 0;

		ConnectorType getConnectorType() { return type; }

		friend class Network;

	protected:
		BaseConnector();

		ConnectorType type;
		ConnectorInfo* connectorInfo; ///< own connector info
		bool isConnected; ///< when start success, set true. if not, set false
	};
}
