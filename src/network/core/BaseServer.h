#pragma once

#include "BaseConnector.h"

namespace CG
{
	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief it is client config. same with connectConfig but it will storage additional server infomation later
	*/
	class ServerConfig : public ConnectConfig
	{
	};

	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief This is a base class to make child server class that can use developers
	*/
	class BaseServer : public BaseConnector
	{
	protected:
		///set server-type
		BaseServer() { type = CONNECTOR_TYPE::CONNECTOR_TYPE_SERVER; }

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief link with client who came from this listener
		*/
		std::map<HostId, ConnectorInfo*> connectorInfoMap;

	public:
		friend class Network;
	};
}