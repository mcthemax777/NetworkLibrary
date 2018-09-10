#pragma once

#include <list>

namespace Util
{
	class Serialize;

	/**
	*
	* @brief test
	* @details test.
	*
	*/
	class BaseSerialize
	{
	public:
		/**
		*
		*        @brief 메서드 간략 설명
		*        @param string a 파라미터 번
		*        @param string b
		*        @return mixed|boolean
		*/
		virtual int serialize(char* data, int dataSize) = 0;
		virtual int deserialize(const char* data, int dataSize) = 0;
		virtual int size() = 0;

	};

	class SerializeInt16 : public BaseSerialize
	{
	private:
		int16_t * pValue;
	public:
		SerializeInt16(int16_t* pValue) : pValue(pValue) {}

	public:
		int serialize(char* data, int dataSize);
		int deserialize(const char* data, int dataSize);
		int size();
	};

	class SerializeInt32 : public BaseSerialize
	{
	private:
		int32_t * pValue;
	public:
		SerializeInt32(int32_t* pValue) : pValue(pValue) {}

	public:
		int serialize(char* data, int dataSize);
		int deserialize(const char* data, int dataSize);
		int size();
	};

	class SerializeInt64 : public BaseSerialize
	{
	private:
		int64_t * pValue;
	public:
		SerializeInt64(int64_t* pValue) : pValue(pValue) {}

	public:
		int serialize(char* data, int dataSize);
		int deserialize(const char* data, int dataSize);
		int size();
	};

	class SerializeString : public BaseSerialize
	{
	private:
		std::string * pValue;
	public:
		SerializeString(std::string* pValue) : pValue(pValue) {}

	public:
		int serialize(char* data, int dataSize);
		int deserialize(const char* data, int dataSize);
		int size();
	};

	class SerializeInt32List : public BaseSerialize
	{
	private:
		std::list<int32_t>* pValueList;
	public:
		SerializeInt32List(std::list<int32_t>* pValueList) : pValueList(pValueList) {}

	public:
		int serialize(char* data, int dataSize);
		int deserialize(const char* data, int dataSize);
		int size();
	};

	template <typename T>
	class SerializePrimitiveType : public BaseSerialize
	{
	private:
		T * pValue;
	public:
		SerializePrimitiveType(T* pValue) : pValue(pValue) {}

	public:
		int serialize(char* data, int dataSize);
		int deserialize(const char* data, int dataSize);
		int size();
	};

	template <typename T>
	int SerializePrimitiveType<T>::serialize(char* data, int dataSize)
	{
		int size = 0;

		memcpy(data, pValue, sizeof(T));

		return sizeof(T);
	}

	template <typename T>
	int SerializePrimitiveType<T>::deserialize(const char* data, int dataSize)
	{
		memcpy(pValue, data, sizeof(T));

		return sizeof(T);
	}

	template <typename T>
	int SerializePrimitiveType<T>::size()
	{
		return sizeof(T);
	}


	template <typename T>
	class SerializePrimitiveTypeList : public BaseSerialize
	{
	private:
		std::list<T>* pValueList;
	public:
		SerializePrimitiveTypeList(std::list<T>* pValueList) : pValueList(pValueList) {}

	public:
		int serialize(char* data, int dataSize);
		int deserialize(const char* data, int dataSize);
		int size();
	};

	template <typename T>
	int SerializePrimitiveTypeList<T>::serialize(char* data, int dataSize)
	{
		int startIndex = 0;

		int32_t size = pValueList->size();

		memcpy(data + startIndex, &size, sizeof(int32_t));

		startIndex += sizeof(int32_t);


		for (T pValue : *pValueList)
		{
			memcpy(data + startIndex, &pValue, sizeof(T));

			startIndex += sizeof(T);
		}

		return startIndex;
	}

	template <typename T>
	int SerializePrimitiveTypeList<T>::deserialize(const char* data, int dataSize)
	{
		int startIndex = 0;

		int32_t size = 0;

		memcpy(&size, data + startIndex, sizeof(int32_t));

		startIndex += sizeof(int32_t);

		for (int i = 0; i < size; i++)
		{
			T value = 0;
			memcpy(&value, data + startIndex, sizeof(T));

			pValueList->push_back(value);

			startIndex += sizeof(T);
		}

		return startIndex;
	}

	template <typename T>
	int SerializePrimitiveTypeList<T>::size()
	{
		return sizeof(int32_t) + pValueList->size() * sizeof(T);
	}


	class Serialize : public BaseSerialize
	{
	private:
		std::list<BaseSerialize*> serializeList;
	public:
		Serialize();
		~Serialize();

		void addMemberValue(bool* value);
		void addMemberValue(char* value);
		void addMemberValue(short* value);
		void addMemberValue(int8_t* value);
		void addMemberValue(int32_t* value);
		void addMemberValue(int64_t* value);
		void addMemberValue(std::string* value);
		void addMemberValue(Serialize* value);

		void addMemberValue(std::list<int32_t>* value);
		void addMemberValue(std::list<Serialize*>* value);

	public:
		int serialize(char* data, int dataSize);
		int deserialize(const char* data, int dataSize);
		int size();

	};


	///////////////////////////////test

	class IntPacket : public Serialize
	{
	public:
		int32_t value;

		IntPacket()
		{
			addMemberValue(&value);
		}
	};

	class TestPacket : public Serialize
	{
	public:
		int32_t id1;
		int32_t id2;
		IntPacket intPacket;
		std::list<int32_t> testList;
		std::list<Serialize*> testList1;
		std::string str;

		TestPacket()
		{
			addMemberValue(&id1);
			addMemberValue(&id2);
			addMemberValue(&intPacket);
			addMemberValue(&testList);
			addMemberValue(&testList1);
			addMemberValue(&str);
		}
	};
}