#include "Serialize.h"

namespace Util
{
	////////////////////////////////////////////////////////SerializeInt16

	int SerializeInt16::serialize(char* data, int dataSize)
	{
		int size = 0;

		memcpy(data, pValue, sizeof(int16_t));

		return sizeof(int16_t);
	}

	int SerializeInt16::deserialize(const char* data, int dataSize)
	{
		memcpy(pValue, data, sizeof(int16_t));

		return sizeof(int16_t);
	}

	int SerializeInt16::size()
	{
		return sizeof(int16_t);
	}	
	
	////////////////////////////////////////////////////////SerializeInt32

	int SerializeInt32::serialize(char* data, int dataSize)
	{
		int size = 0;

		memcpy(data, pValue, sizeof(int32_t));

		return sizeof(int32_t);
	}

	int SerializeInt32::deserialize(const char* data, int dataSize)
	{
		memcpy(pValue, data, sizeof(int32_t));

		return sizeof(int32_t);
	}

	int SerializeInt32::size()
	{
		return sizeof(int32_t);
	}

	////////////////////////////////////////////////////////SerializeInt64

	int SerializeInt64::serialize(char* data, int dataSize)
	{
		int size = 0;

		memcpy(data, pValue, sizeof(int64_t));

		return sizeof(int64_t);
	}

	int SerializeInt64::deserialize(const char* data, int dataSize)
	{
		memcpy(pValue, data, sizeof(int64_t));

		return sizeof(int64_t);
	}

	int SerializeInt64::size()
	{
		return sizeof(int64_t);
	}

	////////////////////////////////////////////////////////SerializeInt32List

	int SerializeInt32List::serialize(char* data, int dataSize)
	{
		int startIndex = 0;

		int32_t size = pValueList->size();

		memcpy(data + startIndex, &size, sizeof(int32_t));

		startIndex += sizeof(int32_t);


		for (int32_t pValue : *pValueList)
		{
			memcpy(data + startIndex, &pValue, sizeof(int32_t));

			startIndex += sizeof(int32_t);
		}

		return startIndex;
	}

	int SerializeInt32List::deserialize(const char* data, int dataSize)
	{
		int startIndex = 0;

		int32_t size = 0;

		memcpy(&size, data + startIndex, sizeof(int32_t));

		startIndex += sizeof(int32_t);

		for (int i = 0; i < size; i++)
		{
			int32_t value = 0;
			memcpy(&value, data + startIndex, sizeof(int32_t));

			pValueList->push_back(value);

			startIndex += sizeof(int32_t);
		}

		return startIndex;
	}

	int SerializeInt32List::size()
	{
		return sizeof(int32_t) + pValueList->size() * sizeof(int32_t);
	}

	////////////////////////////////////////////////////////SerializeString

	int SerializeString::serialize(char* data, int dataSize)
	{
		int startIndex = 0;

		int32_t size = pValue->size();

		memcpy(data + startIndex, &size, sizeof(int32_t));

		startIndex += sizeof(int32_t);

		memcpy(data + startIndex, pValue->c_str(), size);

		startIndex += size;

		return startIndex;
	}

	int SerializeString::deserialize(const char* data, int dataSize)
	{
		int startIndex = 0;

		int32_t size = 0;

		memcpy(&size, data + startIndex, sizeof(int32_t));

		startIndex += sizeof(int32_t);

		pValue->assign(data + startIndex, size);

		startIndex += size;

		return startIndex;
	}

	int SerializeString::size()
	{
		return sizeof(int32_t) + pValue->size();
	}


	Serialize::Serialize()
	{

	}


	Serialize::~Serialize()
	{

	}

	void Serialize::addMemberValue(bool* value)
	{
		SerializePrimitiveType<bool>* spt = new SerializePrimitiveType<bool>(value);
		serializeList.push_back(spt);
	}

	void Serialize::addMemberValue(char* value)
	{
		SerializePrimitiveType<char>* spt = new SerializePrimitiveType<char>(value);
		serializeList.push_back(spt);
	}

	void Serialize::addMemberValue(short* value)
	{
		SerializePrimitiveType<short>* spt = new SerializePrimitiveType<short>(value);
		serializeList.push_back(spt);
	}

	void Serialize::addMemberValue(int8_t* value)
	{
		SerializePrimitiveType<int8_t>* spt = new SerializePrimitiveType<int8_t>(value);
		serializeList.push_back(spt);
	}

	void Serialize::addMemberValue(int32_t* value)
	{
		SerializePrimitiveType<int32_t>* spt = new SerializePrimitiveType<int32_t>(value);
		serializeList.push_back(spt);
	}

	void Serialize::addMemberValue(int64_t* value)
	{
		SerializePrimitiveType<int64_t>* spt = new SerializePrimitiveType<int64_t>(value);
		serializeList.push_back(spt);
	}

	void Serialize::addMemberValue(std::string* value)
	{
		SerializeString* sil = new SerializeString(value);
		serializeList.push_back(sil);
	}

	void Serialize::addMemberValue(Serialize* value)
	{
		serializeList.push_back(value);
	}


	void Serialize::addMemberValue(std::list<int32_t>* value)
	{
		SerializePrimitiveTypeList<int32_t>* sptl = new SerializePrimitiveTypeList<int32_t>(value);
		serializeList.push_back(sptl);
	}

	void Serialize::addMemberValue(std::list<Serialize*>* value)
	{

	}


	int Serialize::serialize(char* data, int dataSize)
	{
		int startIndex = 0;

		for (BaseSerialize *sc : serializeList)
		{
			int serializeSize = sc->serialize(data + startIndex, dataSize - startIndex);

			if (serializeSize > 0)
				startIndex += serializeSize;
			else
				return -1;
		}

		return startIndex;
	}
	int Serialize::deserialize(const char* data, int dataSize)
	{
		int startIndex = 0;

		for (BaseSerialize *sc : serializeList)
		{
			int deserializeSize = sc->deserialize(data + startIndex, dataSize - startIndex);

			if (deserializeSize > 0)
				startIndex += deserializeSize;
			else
				return -1;
		}

		return startIndex;
	}



	int Serialize::size()
	{
		int startIndex = 0;

		for (BaseSerialize *sc : serializeList)
		{
			startIndex += sc->size();

		}

		return startIndex;
	}
}