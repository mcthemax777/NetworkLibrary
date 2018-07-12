#pragma once
#include <cstdint>
#include <string>
#include <list>
#include <functional>
#include "Define.h"

#define CREATE_PACKET(__TYPE__) \
CG::NetworkPacket* create() \
{ \
    return new __TYPE__(); \
} \

namespace CG
{
	typedef int16_t npType_t;
	typedef int32_t npSize_t;

	enum MESSAGE_TYPE
	{
		MESSAGE_TYPE_MESSAGE = 50000,
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

	class Header : public NPSerializer
	{
	public:
		npType_t npType;
		npSize_t npSize;

		int serialize(char* buffer)
		{
			int size = 0;

			memcpy(buffer, &npType, sizeof(npType_t));
			buffer += sizeof(npType_t);
			size += sizeof(npType_t);

			memcpy(buffer, &npSize, sizeof(npSize_t));
			buffer += sizeof(npSize_t);
			size += sizeof(npSize_t);

			return size;
		}

		int deserialize(const char* buffer)
		{
			int size = 0;

			memcpy(&npType, buffer, sizeof(npType_t));
			buffer += sizeof(npType_t);
			size += sizeof(npType_t);

			memcpy(&npSize, buffer, sizeof(npSize_t));
			buffer += sizeof(npSize_t);
			size += sizeof(npSize_t);

			return size;
		}

		int size()
		{
			return sizeof(npType_t) + sizeof(npSize_t);
		}
	};

	class NPString : public NPSerializer
	{
	public:
		int32_t dataSize;
		char* data;

		void init(int _dataSize, char* _data)
		{
			dataSize = _dataSize;

			char* data = new char[dataSize];
			memcpy(data, _data, dataSize);
		}

		int serialize(char* buffer)
		{
			int size = 0;

			memcpy(buffer, &dataSize, sizeof(int32_t));
			buffer += sizeof(int32_t);
			size += sizeof(int32_t);

			memcpy(buffer, data, dataSize);
			buffer += dataSize;
			size += dataSize;

			return size;
		}

		int deserialize(const char* buffer)
		{
			int size = 0;

			memcpy(&dataSize, buffer, sizeof(int32_t));
			buffer += sizeof(int32_t);
			size += sizeof(int32_t);

			memcpy(data, buffer, dataSize);
			buffer += dataSize;
			size += dataSize;

			return size;
		}

		int size()
		{
			return sizeof(npType_t) + sizeof(npSize_t);
		}

		~NPString()
		{
			delete data;
		}
	};

	class NetworkPacket : public NPSerializer
	{
	public:
		NetworkPacket() { npsList.push_back(&header); }

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
		}

		virtual NetworkPacket* create()
		{
			return new NetworkPacket();
		}

	public:
		Header header;

		void* onReceiveNPacket;
	};

	class MessagePacket : public NetworkPacket
	{
	public:
		MessagePacket()
		{
			header.npType = MESSAGE_TYPE_MESSAGE;
			npsList.push_back(&str);
		}

		CREATE_PACKET(CG::MessagePacket)

	public:
		NPString str;
	};
}
