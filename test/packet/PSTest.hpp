#pragma once

#include "network/module/packet/CGServer.h"
#include "Packet.hpp"
#include "util/log/Log.h"

int PSTest()
{
	CG::CGServer* server = new CG::CGServer();

	CG::ServerConfig serverConfig;

	serverConfig.ip = "127.0.0.1";
	serverConfig.port = 8080;

	server->onConnect = [](CG::HostId hostId)
	{
		//std::cout << "connected with client";
		InfoLog("connected with client");
	};

	server->onDisconnect = [](CG::HostId hostId)
	{
		//std::cout << "disconnected with client";
		InfoLog("disconnected with client");
	};

	server->registerPacket<TestPacket>([](CG::HostId hostId, TestPacket* packet) {
		//print receive message
		InfoLog("receive data from client = %d %s %d %d %s %d\n",
			packet->id1,
			packet->str.c_str(),
			packet->intPacket.value,
			packet->testList.back(),
			packet->testList1.back().c_str(),
			packet->testList2.back()->value);
	});

	server->registerPacket<MessagePacket>([server](CG::HostId hostId, MessagePacket* packet) {
		//print receive message
		//std::cout << "receive data from client " << packet->str;
		InfoLog("receive data from client - %s", packet->str.c_str());

		//send packet
		MessagePacket sendPacket;
		sendPacket.str = "message packet!!!";
		server->sendPacket(hostId, &sendPacket);
	});

	server->start(&serverConfig);

	//server->stop();

	return 0;
}