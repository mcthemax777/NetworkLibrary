#pragma once

#include "BaseConnector.h"

namespace CG
{
	class ClientConfig : public ConnectConfig
	{
	};

	class BaseClient : public BaseConnector
	{
	public:
		BaseClient() { type = CONNECTOR_TYPE::CONNECTOR_TYPE_CLIENT; }
		void processMessage();
	};
}
