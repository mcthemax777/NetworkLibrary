#include "network/module/basic/Server.h"
#include <iostream>
#include <memory>

int main(void)
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

	server->onReceive = [&](CG::HostId hostId, char* data, int dataSize)
	{
		//print receive message
		char* receiveData = new char[dataSize];
		
		memcpy(receiveData, data, dataSize);
		receiveData[dataSize] = 0;

		std::cout << "receive data from client " << receiveData;
	};
	
	/*
	char* strt = "hellohello";
	server->sendMessage(hostId, strt, 10);
	*/
	
	server->start(&serverConfig);

	//server->stop();

	getchar();

	return 0;
}








