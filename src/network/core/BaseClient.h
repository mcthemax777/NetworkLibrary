#pragma once

#include "BaseConnector.h"

namespace CG
{
	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief it is client config. same with connectConfig but it will storage additional client infomation later
	*/
	class ClientConfig : public ConnectConfig
	{
	};

	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief This is a base class to make child client class that can use developers
	*/
	class BaseClient : public BaseConnector
	{
	protected:
		///set client-type 
		BaseClient() { type = CONNECTOR_TYPE::CONNECTOR_TYPE_CLIENT; }

		//void processMessage();
	};
}
