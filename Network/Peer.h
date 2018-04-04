#pragma once

#include "Connector.h"

namespace CG
{
	class Peer : public Connector
	{
	public:
		Peer() { type = CONNECTOR_TYPE::CONNECTOR_TYPE_CLIENT; }
		void sendMessage(const char* data, int dataSize);

	};
}
