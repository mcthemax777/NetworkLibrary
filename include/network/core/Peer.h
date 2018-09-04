#pragma once

#include "BaseClient.h"

namespace CG
{
	class Peer : public BaseClient
	{
	public:
		Peer() { type = CONNECTOR_TYPE::CONNECTOR_TYPE_CLIENT; }
		void sendMessage(const char* data, int dataSize);

	};
}
