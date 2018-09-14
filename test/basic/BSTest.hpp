#pragma once

#include "network/module/basic/Server.h"
#include <iostream>
#include <memory>

int BSTest(void)
{
	CG::Server* server = new CG::Server();

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

	server->onReceive = [server](CG::HostId hostId, char* data, int dataSize)
	{
		//print receive message
		std::string str(data, dataSize);

		std::cout << "receive data from client " << str;
	};
	
	/*
	std::string str(data, dataSize);
	server->sendMessage(hostId, str);

	char* strt = "hellohello";
	server->sendMessage(hostId, strt, 10);
	*/
	
	server->start(&serverConfig);

	//server->stop();

	return 0;
}








