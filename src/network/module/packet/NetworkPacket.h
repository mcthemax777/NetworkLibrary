#pragma once
#include <cstdint>
#include <string>
#include <list>
#include <functional>
#include <limits>
#include "network/core/NetworkDefine.h"
#include "Util/Serialize/Serialize.h"

namespace CG
{
	typedef int16_t npType_t;
	typedef int32_t npSize_t;

	const uint16_t MAX_NETWORK_PACKET_TYPE = 30000;

	enum NETWORK_PACKET_TYPE
	{
		NETWORK_PACKET_COUNT = 30000
	};

	typedef int16_t inpType_t;

	class Header : public Util::Serialize
	{
	public:
		npType_t npType;
		npSize_t npSize;

		Header()
		{
			addMemberValue(&npType);
			addMemberValue(&npSize);
		}
	};

	class NetworkPacket : public Util::Serialize
	{
	public:
		NetworkPacket() 
		{ 
			addMemberValue(&header);
		}

		void setType(npType_t type) { header.npType = type; }

		virtual NetworkPacket* create()
		{
			return new NetworkPacket();
		}

		void setPacketSize()
		{
			header.npSize = size();
		}

	public:
		Header header;
	};
}
