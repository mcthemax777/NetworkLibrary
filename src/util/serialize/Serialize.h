#pragma once

#include <list>
#include <typeinfo>
#include <iostream>
#include <string>


namespace Util
{
	class Serialize;

	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief interface to serialize/deserialize
	*/
	class BaseSerialize
	{
	public:
		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief serialize member value to data
		* @param char* data : storage to save member value
		* @param int dataSize : max data's size to serialize
		* @return int result : serialized data's size
		*/
		virtual int serialize(char* data, int dataSize) = 0;

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief deserialize member value to data
		* @param char* data : storaged data to deserialize
		* @param int dataSize : data's size to deserialize
		* @return int result : deserialized data's size
		*/
		virtual int deserialize(const char* data, int dataSize) = 0;

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief whole member value data size
		*/
		virtual int size() = 0;

	};

	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief primmitive type's storage to serialize/deserialize
	*/
	template <typename T>
	class SerializePrimitiveType : public BaseSerialize
	{
	private:
		T * pValue;
	public:
		SerializePrimitiveType(T* pValue) : pValue(pValue) {}

	public:
		///see perant
		int serialize(char* data, int dataSize);

		///see perant
		int deserialize(const char* data, int dataSize);
		
		///see perant
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

	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief std::string type's storage to serialize/deserialize
	*/
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

	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief primmitive type list's storage to serialize/deserialize
	*/
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

		//copy list size
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

		//copy list size
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

	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief std::string type list's storage to serialize/deserialize
	*/
	class SerializeStringList : public BaseSerialize
	{
	private:
		std::list<std::string>* pValueList;
	public:
		SerializeStringList(std::list<std::string>* pValueList) : pValueList(pValueList) {}

	public:
		int serialize(char* data, int dataSize);
		int deserialize(const char* data, int dataSize);
		int size();
	};

	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief serialization type list's storage to serialize/deserialize
	*/
	template<typename T>
	class SerializeList : public BaseSerialize
	{
	private:
		std::list<T*>* pValueList;
	public:
		SerializeList(std::list<T*>* pValueList) : pValueList(pValueList) {}

	public:
		int serialize(char* data, int dataSize);
		int deserialize(const char* data, int dataSize);
		int size();
	};


	template<typename T>
	int SerializeList<T>::serialize(char* data, int dataSize)
	{
		int startIndex = 0;

		int32_t size = pValueList->size();

		//copy list size
		memcpy(data + startIndex, &size, sizeof(int32_t));

		startIndex += sizeof(int32_t);

		for (T* pValue : *pValueList)
		{
			pValue->serialize(data + startIndex, dataSize - startIndex);

			startIndex += pValue->size();
		}

		return startIndex;
	}

	template<typename T>
	int SerializeList<T>::deserialize(const char* data, int dataSize)
	{
		int startIndex = 0;

		int32_t size = 0;

		//copy list size
		memcpy(&size, data + startIndex, sizeof(int32_t));

		startIndex += sizeof(int32_t);

		for (int i = 0; i < size; i++)
		{
			T* s = new T();
			s->deserialize(data + startIndex, dataSize - startIndex);

			pValueList->push_back(s);

			startIndex += s->size();
		}

		return startIndex;
	}

	template<typename T>
	int SerializeList<T>::size()
	{
		int size = sizeof(int32_t);

		for (Serialize* pValue : *pValueList)
		{
			size += pValue->size();
		}

		return size;
	}


	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief serialization type's storage to serialize/deserialize
	*/
	class Serialize : public BaseSerialize
	{
	private:
		///member values's storage 
		std::list<BaseSerialize*> serializeList;
	public:
		Serialize() {};
		~Serialize() {};

		///add bool member value
		void addMemberValue(bool* value);
		
		///add char member value
		void addMemberValue(char* value);

		///add short member value
		void addMemberValue(short* value);

		///add int8_t member value
		void addMemberValue(int8_t* value);

		///add int32_t member value
		void addMemberValue(int32_t* value);

		///add int64_t member value
		void addMemberValue(int64_t* value);

		///add std::string member value
		void addMemberValue(std::string* value);

		///add Serialize member value
		void addMemberValue(Serialize* value);

		///add std::list<int32_t> member value
		void addMemberValue(std::list<int32_t>* value);

		///add std::list<std::string> member value
		void addMemberValue(std::list<std::string>* value);

		///add std::list<Serialize*> member value
		template<typename T, typename std::enable_if<std::is_base_of<Util::Serialize, T>::value>::type* = nullptr>
		void addMemberValue(std::list<T*>* value);

	public:

		///see parent
		int serialize(char* data, int dataSize);

		///see parent
		int deserialize(const char* data, int dataSize);

		///see parent
		int size();

	};

	template<typename T, typename std::enable_if<std::is_base_of<Util::Serialize, T>::value>::type* = nullptr>
	void Serialize::addMemberValue(std::list<T*>* value)
	{
		SerializeList<T>* sl = new SerializeList<T>(value);
		serializeList.push_back(sl);
	}
}