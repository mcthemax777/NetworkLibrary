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
		virtual int serialize(char* data) = 0;
		virtual int deserialize(const char* data) = 0;
		virtual int size() = 0;

	};

	class SerializeInt16 : public BaseSerialize
	{
	private:
		int16_t * pValue;
	public:
		SerializeInt16(int16_t* pValue) : pValue(pValue) {}

	public:
		int serialize(char* data);
		int deserialize(const char* data);
		int size();
	};

	class SerializeInt32 : public BaseSerialize
	{
	private:
		int32_t * pValue;
	public:
		SerializeInt32(int32_t* pValue) : pValue(pValue) {}

	public:
		int serialize(char* data);
		int deserialize(const char* data);
		int size();
	};

	class SerializeInt64 : public BaseSerialize
	{
	private:
		int64_t * pValue;
	public:
		SerializeInt64(int64_t* pValue) : pValue(pValue) {}

	public:
		int serialize(char* data);
		int deserialize(const char* data);
		int size();
	};

	class SerializeString : public BaseSerialize
	{
	private:
		std::string * pValue;
	public:
		SerializeString(std::string* pValue) : pValue(pValue) {}

	public:
		int serialize(char* data);
		int deserialize(const char* data);
		int size();
	};

	class SerializeInt32List : public BaseSerialize
	{
	private:
		std::list<int32_t>* pValueList;
	public:
		SerializeInt32List(std::list<int32_t>* pValueList) : pValueList(pValueList) {}

	public:
		int serialize(char* data);
		int deserialize(const char* data);
		int size();
	};

	class Serialize : public BaseSerialize
	{
	private:
		std::list<BaseSerialize*> serializeList;
	public:
		Serialize();
		~Serialize();

		void addMemberValue(int16_t* value);
		void addMemberValue(int32_t* value);
		void addMemberValue(int64_t* value);
		void addMemberValue(std::string* value);
		void addMemberValue(Serialize* value);

		void addMemberValue(std::list<int32_t>* value);
		void addMemberValue(std::list<Serialize*>* value);

	public:
		int serialize(char* data);
		int deserialize(const char* data);
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
		std::string str;

		TestPacket()
		{
			addMemberValue(&id1);
			addMemberValue(&id2);
			addMemberValue(&intPacket);
			addMemberValue(&testList);
			addMemberValue(&str);
		}
	};
}