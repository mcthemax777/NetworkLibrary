#pragma once
#include <cstdint>
#include <string>
#include <list>
#include <functional>
#include "Define.h"
#include "Util/Serializer/Serializer.h"

#define CREATE_PACKET(__TYPE__) \
__TYPE__* create() \
{ \
    return new __TYPE__(); \
} \

namespace CG
{
	typedef int16_t npType_t;
	typedef int32_t npSize_t;

	enum MESSAGE_TYPE
	{
		MESSAGE_TYPE_MESSAGE = 1000,
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

	public:
		Header header;
	};

	class MessagePacket : public NetworkPacket
	{
	public:
		MessagePacket()
		{
			//set type
			header.npType = MESSAGE_TYPE_MESSAGE;
			
			//init serial member value
			addMemberValue(&str);
		}

		CREATE_PACKET(CG::MessagePacket)

	public:
		std::string str;
	};
}
