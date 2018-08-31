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
/*
		int serialize(char* buffer)
		{
			int size = 0;

			char* pBuffer = buffer;

			std::list<NPSerializer*>::iterator itr;
			itr = npsList.begin();

			for (itr = npsList.begin(); itr != npsList.end(); itr++)
			{
				int resultSize = (*itr)->serialize(pBuffer);
				pBuffer += resultSize;
				size += resultSize;
			}

			return size;
		}

		int deserialize(const char* buffer)
		{
			int size = 0;

			const char* pBuffer = buffer;

			std::list<NPSerializer*>::iterator itr;
			itr = npsList.begin();

			for (itr = npsList.begin(); itr != npsList.end(); itr++)
			{
				int resultSize = (*itr)->deserialize(pBuffer);
				pBuffer += resultSize;
				size += resultSize;
			}

			return size;
		}

		virtual int size()
		{
			int size = 0;

			std::list<NPSerializer*>::iterator itr;
			itr = npsList.begin();

			for (itr = npsList.begin(); itr != npsList.end(); itr++)
			{
				size += (*itr)->size();
			}

			return size;
		}*/

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
			header.npType = MESSAGE_TYPE_MESSAGE;
			addMemberValue(&str);
		}

		CREATE_PACKET(CG::MessagePacket)

	public:
		std::string str;
	};
}
