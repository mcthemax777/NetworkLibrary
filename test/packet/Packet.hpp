#pragma once

#include "network/module/packet/NetworkPacket.h"

enum NETWORK_PACKET_TYPE
{
	NETWORK_PACKET_TYPE_TEST_PACKET,
	NETWORK_PACKET_TYPE_MESSAGE
};

class TestIntData : public Util::Serialize
{
public:
	int32_t value;

	TestIntData()
	{
		addMemberValue(&value);
	}
};

class TestPacket : public CG::NetworkPacket
{
public:
	int32_t id1;
	std::string str;
	TestIntData intPacket;
	std::list<int32_t> testList;
	std::list<std::string> testList1;
	std::list<TestIntData*> testList2;

	TestPacket()
	{
		//set type
		setType(NETWORK_PACKET_TYPE_TEST_PACKET);

		//init serial member value
		addMemberValue(&id1);
		addMemberValue(&str);
		addMemberValue(&intPacket);
		addMemberValue(&testList);
		addMemberValue(&testList1);
		addMemberValue(&testList2);
	}
};

class MessagePacket : public CG::NetworkPacket
{
public:
	MessagePacket()
	{
		//set type
		setType(NETWORK_PACKET_TYPE_MESSAGE);

		//init serial member value
		addMemberValue(&str);
	}

public:
	std::string str;
};