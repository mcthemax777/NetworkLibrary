#include "Serialize.h"

namespace Util
{

	////////////////////////////////////////////////////////SerializeString




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
		SerializeList<int32_t>* sptl = new SerializeList<int32_t>(value);
		serializeList.push_back(sptl);
	}

	void Serialize::addMemberValue(std::list<Serialize*>* value)
	{
		SerializeList<Serialize*>* sptl = new SerializeList<Serialize*>(value);
		serializeList.push_back(sptl);
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