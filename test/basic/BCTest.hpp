#pragma once

#include "network/module/basic/Client.h"
#include "log/Log.h"

int BCTest(void)
{
	CG::Client* client = new CG::Client();

	CG::ServerConfig clientConfig;

	clientConfig.ip = "127.0.0.1";
	clientConfig.port = 8080;

	client->onConnect = [client](CG::HostId hostId)
	{
		//std::cout << "connected with server";
		DebugLog("connected with server");

		char* strt = "hellohello";
		client->sendMessage(strt, 10);
	};

	client->onDisconnect = [](CG::HostId hostId)
	{
		//std::cout << "disconnected with server";
		DebugLog("disconnected with server");

	};

	client->onReceive = [client](CG::HostId hostId, char* data, int dataSize)
	{
		//print receive message
		std::string str(data, dataSize);

		//std::cout << "receive data from client " << str;
		DebugLog("receive data from server - %s", str.c_str());
	};

	/*
	char* strt = "hellohello";
	client->sendMessage(strt, 10);
	*/

	client->start(&clientConfig);

	//client->stop();
	
	return 0;
}