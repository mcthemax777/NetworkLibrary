#pragma once
#include <cstdint>
#include <string>
#include <list>
#include <functional>
#include <limits>
#include "network/core/NetworkDefine.h"
#include "Util/Serialize/Serialize.h"

#define CREATE_PACKET(__TYPE__) \
__TYPE__* create() \
{ \
    return new __TYPE__(); \
} \

namespace CG
{
	typedef int16_t npType_t;
	typedef int32_t npSize_t;

	const uint16_t MAX_NETWORK_PACKET_TYPE = 30000;

	enum NETWORK_PACKET_TYPE
	{
		NETWORK_PACKET_TYPE_MESSAGE,
		NETWORK_PACKET_COUNT
	};

	typedef int16_t inpType_t;

	class NPSerializer
	{
	public:
		std::list<NPSerializer*> npsList;
		virtual int serialize(char* buffer) = 0;
		virtual int deserialize(const char* buffer) = 0;
		virtual int size() = 0;
	};

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

		virtual NetworkPacket* create()
		{
			return new NetworkPacket();
		}

		void setPacketSize()
		{
			header.npSize = size() - sizeof(npType_t) - sizeof(npSize_t);
		}

	public:
		Header header;
	};

	class MessagePacket : public NetworkPacket
	{
	public:
		MessagePacket()
		{
			//set type
			header.npType = NETWORK_PACKET_TYPE_MESSAGE;
			
			//init serial member value
			addMemberValue(&str);
		}

		CREATE_PACKET(CG::MessagePacket)

	public:
		std::string str;
	};
}
