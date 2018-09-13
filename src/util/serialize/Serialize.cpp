#include "Serialize.h"

namespace Util
{
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







	int SerializeStringList::serialize(char* data, int dataSize)
	{
		int startIndex = 0;

		int32_t size = pValueList->size();

		memcpy(data + startIndex, &size, sizeof(int32_t));

		startIndex += sizeof(int32_t);


		for (auto pValue : *pValueList)
		{
			SerializeString ss(&pValue);
			ss.serialize(data + startIndex, dataSize - startIndex);

			startIndex += ss.size();
		}

		return startIndex;
	}

	int SerializeStringList::deserialize(const char* data, int dataSize)
	{
		int startIndex = 0;

		int32_t size = 0;

		memcpy(&size, data + startIndex, sizeof(int32_t));

		startIndex += sizeof(int32_t);

		for (int i = 0; i < size; i++)
		{
			std::string value;

			SerializeString ss(&value);
			ss.deserialize(data + startIndex, dataSize - startIndex);

			pValueList->push_back(value);

			startIndex += ss.size();
		}

		return startIndex;
	}

	int SerializeStringList::size()
	{

		int size = sizeof(int32_t);

		for (std::string pValue : *pValueList)
		{
			SerializeString ss(&pValue);

			size += ss.size();
		}

		return size;

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
		SerializePrimitiveTypeList<int32_t>* sl = new SerializePrimitiveTypeList<int32_t>(value);
		serializeList.push_back(sl);
	}

	void Serialize::addMemberValue(std::list<std::string>* value)
	{
		SerializeStringList* sl = new SerializeStringList(value);
		serializeList.push_back(sl);
	}



	int Serialize::serialize(char* data, int dataSize)
	{
		int startIndex = 0;

		for (BaseSerialize *sc : serializeList)
		{
			startIndex += sc->serialize(data + startIndex, dataSize - startIndex);

		}

		return startIndex;
	}

	int Serialize::deserialize(const char* data, int dataSize)
	{
		int startIndex = 0;

		for (BaseSerialize *sc : serializeList)
		{
			startIndex += sc->deserialize(data + startIndex, dataSize - startIndex);

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