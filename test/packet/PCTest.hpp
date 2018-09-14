#pragma once

#include "network/module/packet/CGClient.h"
#include "Packet.hpp"
#include "log/Log.h"

int PCTest()
{
	CG::CGClient* client = new CG::CGClient();

	CG::ClientConfig serverConfig;

	serverConfig.ip = "127.0.0.1";
	serverConfig.port = 8080;

	client->onConnect = [client](CG::HostId hostId)
	{
		//std::cout << "connected with server";
		DebugLog("connected with server");


		MessagePacket sendPacket;
		sendPacket.str = "message packet!!!";
		client->sendPacket(&sendPacket);
	};

	client->onDisconnect = [](CG::HostId hostId)
	{
		//std::cout << "disconnected with server";
		DebugLog("disconnected with server");
	};

	client->registerPacket<TestPacket>([](CG::HostId hostId, TestPacket* packet) {
		//print receive packet
		DebugLog("receive data from client = %d %s %d %d %s %d\n",
			packet->id1,
			packet->str.c_str(),
			packet->intPacket.value,
			packet->testList.back(),
			packet->testList1.back().c_str(),
			packet->testList2.back()->value);
	});

	client->registerPacket<MessagePacket>([client](CG::HostId hostId, MessagePacket* packet) {
		//print receive packet
		//std::cout << "receive data from server " << packet->str;
		DebugLog("receive data from server - %s", packet->str.c_str());

		//send test packet
		TestPacket p;
		p.id1 = 100;
		p.str = "test";
		p.intPacket.value = 300;
		p.testList.push_back(400);
		p.testList.push_back(500);
		p.testList1.push_back("string1");
		p.testList1.push_back("string2");

		TestIntData tid1;
		tid1.value = 600;

		TestIntData tid2;
		tid2.value = 700;

		p.testList2.push_back(&tid1);
		p.testList2.push_back(&tid2);

		client->sendPacket(&p);
	});

	client->start(&serverConfig);

	//server->stop();

	return 0;
}