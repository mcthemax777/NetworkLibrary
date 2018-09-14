#pragma once


#include "network/module/packet/CGServer.h"
#include "network/module/packet/NetworkPacket.h"
#include "Packet.hpp"
#include <iostream>
#include <memory>

using namespace CG;
int PSTest()
{
	CG::CGServer* server = new CG::CGServer();

	CG::ServerConfig serverConfig;

	serverConfig.ip = "127.0.0.1";
	serverConfig.port = 8080;

	server->onConnect = [](CG::HostId hostId)
	{
		std::cout << "connected with client";
	};

	server->onDisconnect = [](CG::HostId hostId)
	{
		std::cout << "disconnected with client";
	};

	server->registerPacket<TestPacket>([](CG::HostId hostId, TestPacket* packet) {
		//print receive message
		printf("receive data from client = %d %s %d %d %s %d\n",
			packet->id1,
			packet->str.c_str(),
			packet->intPacket.value,
			packet->testList.back(),
			packet->testList1.back().c_str(),
			packet->testList2.back()->value);
	});

	server->registerPacket<MessagePacket>([server](CG::HostId hostId, MessagePacket* packet) {
		//print receive message
		std::cout << "receive data from client " << packet->str;

		MessagePacket sendPacket;
		server->sendPacket(hostId, &sendPacket);
	});

	/*
	char* strt = "hellohello";
	server->sendMessage(hostId, strt, 10);
	*/

	server->start(&serverConfig);

	//server->stop();

	return 0;
}