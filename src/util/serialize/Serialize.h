#pragma once

#include <list>

namespace Util
{
	class Serialize;

	template<typename T>
	struct type_name
	{
		static const char* name() { static_assert(false, "You are missing a DECL_TYPE_NAME"); }
	};

	template<> struct type_name<bool> { static const char* name() { return "bool"; } };
	template<> struct type_name<char> { static const char* name() { return "char"; } };
	template<> struct type_name<short> { static const char* name() { return "short"; } };
	template<> struct type_name<int8_t> { static const char* name() { return "int8_t"; } };
	template<> struct type_name<int32_t> { static const char* name() { return "int32_t"; } };
	template<> struct type_name<int64_t> { static const char* name() { return "int64_t"; } };
	template<> struct type_name<std::string> { static const char* name() { return "string"; } };
	template<> struct type_name<Serialize*> { static const char* name() { return "Serialize"; } };

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


	template <typename T>
	class SerializeList : public BaseSerialize
	{
	private:
		std::list<T>* pValueList;
	public:
		SerializeList(std::list<T>* pValueList) : pValueList(pValueList) {}

	public:
		int serialize(char* data, int dataSize);
		int deserialize(const char* data, int dataSize);
		int size();
	};

	template <typename T>
	int SerializeList<T>::serialize(char* data, int dataSize)
	{
		int startIndex = 0;

		int32_t size = pValueList->size();

		memcpy(data + startIndex, &size, sizeof(int32_t));

		startIndex += sizeof(int32_t);


		//get T's type
		std::string name = typeid(T).name();

		//T = std::string
		if (name.compare(typeid(std::string).name()) == 0)
		{
			for (T pValue : *pValueList)
			{
				SerializeString ss(pValue);
				ss.serialize(data + startIndex, data - startIndex);

				startIndex += ss.size();
			}
		}
		else if (name.compare(typeid(Serialize*).name()) == 0) //T = Serialize*
		{
			for (T pValue : *pValueList)
			{
				Serialize* s = pValue;
				s->serialize(data + startIndex, data - startIndex);

				startIndex += s.size();
			}
		}
		else //T = primitive type
		{
			for (T pValue : *pValueList)
			{
				memcpy(data + startIndex, &pValue, sizeof(T));

				startIndex += sizeof(T);
			}
		}

		return startIndex;
	}

	template <typename T>
	int SerializeList<T>::deserialize(const char* data, int dataSize)
	{
		int startIndex = 0;

		int32_t size = 0;

		memcpy(&size, data + startIndex, sizeof(int32_t));

		startIndex += sizeof(int32_t);



		//get T's type
		std::string name = typeid(T).name();

		//T = std::string
		if (name.compare(typeid(std::string).name()) == 0)
		{
			for (int i = 0; i < size; i++)
			{
				T value = 0;

				SerializeString ss(value);
				ss.deserialize(data + startIndex, data - startIndex);

				pValueList->push_back(value);

				startIndex += ss.size();
			}
		}
		else if (name.compare(typeid(Serialize*).name()) == 0) //T = Serialize*
		{
			for (int i = 0; i < size; i++)
			{
				T value = 0;

				Serialize* s = value;
				s->deserialize(data + startIndex, data - startIndex);

				pValueList->push_back(s);

				startIndex += s.size();
			}
		}
		else //T = primitive type
		{
			for (int i = 0; i < size; i++)
			{
				T value = 0;
				memcpy(&value, data + startIndex, sizeof(T));

				pValueList->push_back(value);

				startIndex += sizeof(T);
			}
		}

		return startIndex;
	}

	template <typename T>
	int SerializeList<T>::size()
	{
		//get T's type
		std::string name = typeid(T).name();

		//T = std::string
		if (name.compare(typeid(std::string).name()) == 0)
		{
			int size = sizeof(int32_t);

			for (T pValue : *pValueList)
			{
				SerializeString ss(pValue);

				size += ss->size();
			}

			return size;
		}
		else if (name.compare(typeid(Serialize*).name()) == 0) //T = Serialize*
		{
			int size = sizeof(int32_t);

			for (T pValue : *pValueList)
			{
				Serialize* s = pValue;

				size += s->size();
			}

			return size;
		}
		else //T = primitive type
		{
			return sizeof(int32_t) + pValueList->size() * sizeof(T);
		}
		
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