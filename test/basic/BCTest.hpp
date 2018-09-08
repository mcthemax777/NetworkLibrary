#pragma once

#include "network/module/basic/Client.h"
#include <iostream>
#include <memory>

int BCTest(void)
{
	CG::Client* client = new CG::Client();

	CG::ClientConfig clientConfig;

	clientConfig.ip = "127.0.0.1";
	clientConfig.port = 8080;

	client->onConnect = [&](CG::HostId hostId)
	{
		std::cout << "connected with client";

		char* strt = "hellohello";
		client->sendMessage(strt, 10);
	};

	client->onDisconnect = [](CG::HostId hostId)
	{
		std::cout << "disconnected with client";
	};

	client->onReceive = [&](CG::HostId hostId, char* data, int dataSize)
	{
		//print receive message
		char* receiveData = new char[dataSize];

		memcpy(receiveData, data, dataSize);
		receiveData[dataSize] = 0;

		std::cout << "receive data from client " << receiveData;
	};

	/*
	char* strt = "hellohello";
	client->sendMessage(hostId, strt, 10);
	*/

	client->start(&clientConfig);

	//client->stop();
	
	return 0;
}