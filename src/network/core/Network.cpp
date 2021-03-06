#include "Network.h"
#include <sstream>
#include <chrono>
#if OS_PLATFORM == PLATFORM_WINDOWS

#else

#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <signal.h>
#include <cstring>


#endif

#include "WorkerThread.h"
#include "util/log/Log.h"
#include "BaseConnector.h"
#include "ConnectorInfo.h"
#include "NetworkDefine.h"
#include "BaseServer.h"
#include "BaseClient.h"
#include "Timer.h"
#include "util/parser/FileParser.h"



namespace CG
{
	WorkerThread* Network::getWorkerThreadUsingHash(int hashKey)
	{
		//single thread
		if (workerThreadCount == 0)
		{
			return (*workerThreadArray);
		}
		else //multi thread
		{
			//now just make by %
			int hash = hashKey % workerThreadCount;

			return workerThreadArray[hash];
		}
	}

	bool Network::processReceiveData(ConnectorInfo* connectorInfo)
	{
		//receive processing worker thread
		WorkerThread* workerThread = getWorkerThreadUsingHash(connectorInfo->hostId);

		//receive buffer in buffer pool in worker thread
		Buffer* buffer = workerThread->bufferPool->getObject();

		//receive data
		int dataSize = recv(connectorInfo->getHostId(), buffer->data, RCV_BUF, 0);

		if (dataSize > 0) //if received data
		{
			DebugLog("received data size - %d", dataSize);

			//set buffer member
			buffer->startIndex = 0;
			buffer->dataSize = dataSize;

			//create data packet to deliver to worker thread
			DataPacket* dp = workerThread->dataPacketPool->getObject();

			dp->setDataPacket(RECEIVE_TYPE_DATA, connectorInfo, buffer);

			//push data packet in queue and wake up worker thread
			workerThread->pushDataPacket(dp);

			return true;
		}
		else //if disconnect
		{
			//buffer return because not exist received data
			workerThread->bufferPool->returnObject(buffer);

			//send disconnecting packet to worker thread
			disconnectWithConnectorInfo(workerThread, connectorInfo);

			//ToDo. no disconnect, error, process error
			if (dataSize != -1)
			{
				ErrorLog("receive count - %d", dataSize);
			}

			return false;
		}
	}

#if OS_PLATFORM == PLATFORM_WINDOWS

	//remove when develop iocp
	void Network::windowsConnectorInfoThread(ConnectorInfo* connectorInfo)
	{
		WorkerThread* workerThread = getWorkerThreadUsingHash(connectorInfo->hostId);

		DataPacket* dp = workerThread->dataPacketPool->getObject();

		dp->setDataPacket(RECEIVE_TYPE_CONNECT, connectorInfo);

		workerThread->pushDataPacket(dp);

		while (true)
		{
			bool isReceiveData = processReceiveData(connectorInfo);

			if (isReceiveData == false)
			{
				break;
			}
		}
	}

	//remove when develop iocp
	void Network::windowsServerThread(BaseServer* server)
	{
		struct sockaddr_in clntaddr;
		int clntaddrLen = sizeof(clntaddr);

		while (true)
		{
			SOCKET clientSocket = accept(server->connectorInfo->hostId, (SOCKADDR*)&clntaddr, &clntaddrLen);

			if (clientSocket == INVALID_SOCKET)
			{
				ErrorLog("accept error()");
				break;
			}

			ConnectorInfo* connectorInfo = connectorInfoPool->getObject();

			connectorInfo->init(server, clientSocket);


			if (server->connectorInfoMap.insert(std::pair<int, ConnectorInfo*>(connectorInfo->hostId, connectorInfo)).second == false)
			{
				ErrorLog("insert error");
				return;
			}

			std::thread* clientThread = new std::thread(&Network::windowsConnectorInfoThread, this, connectorInfo);

			clientThreadList->push_back(clientThread);

		}
	}


#else
	void Network::setSocketOption(int fd)
	{
		int status = fcntl(fd, F_SETFL, fcntl(clntFd, F_GETFL, 0) | O_NONBLOCK);

		if (status == -1)
		{
			ErrorLog("calling fcntl");
		}

		// LINGER 구조체의 값 설정
		struct linger ling = { 0, };
		ling.l_onoff = 1;   // LINGER 옵션 사용 여부
		ling.l_linger = 0;  // LINGER Timeout 설정

		setsockopt(fd, SOL_SOCKET, SO_LINGER, (char*)&ling, sizeof(ling)); // LINGER 옵션을 Socket에 적용
	}

#endif

	Network::Network()
	{
		//get network setting from file
		Util::FileParser fp;
		std::unordered_map<std::string, std::string> kv = fp.parseSettingFile("network_config.cg");

		//get worker thread count 
		std::string workerThreadCountStr = kv["worker_thread_count"];

		std::stringstream ss(workerThreadCountStr);

		if ((ss >> workerThreadCount).fail())
		{
			ErrorLog("worker_thread_count is not int-type");
			ErrorLog("worker_thread_count do not exist");
		}


		if (workerThreadCount == 0) //single Thread
		{
			workerThreadArray = new WorkerThread*();

			*workerThreadArray = new WorkerThread(false);

			if ((*workerThreadArray)->initialize() == false)
			{
				ErrorLog("worker thread error");
				return;
			}
		}
		else //multi thread
		{
			workerThreadArray = new WorkerThread*[workerThreadCount];

			//create worker thread and init and run
			for (int i = 0; i < workerThreadCount; i++)
			{
				workerThreadArray[i] = new WorkerThread(true);

				if (workerThreadArray[i]->initialize() == false)
				{
					ErrorLog("worker thread error");
					return;
				}

				workerThreadArray[i]->start();
			}
		}

		clientList = new Util::MTList<BaseClient*>();
		serverList = new Util::MTList<BaseServer*>();
		//eventFunctionList = new Util::MTList<EventFunction*>();
		timerQueue = new Util::NBQueue<Timer*>();

		connectorInfoPool = new Util::ObjectPool<ConnectorInfo>(100, true);

		//init network function
		init();
	}

	Network::~Network()
	{

#if OS_PLATFORM == PLATFORM_WINDOWS

		WSACleanup();

		for (int i = 0; i < serverThreadList->size(); i++)
		{
			delete serverThreadList->at(i);

		}

		for (int i = 0; i < clientThreadList->size(); i++)
		{
			delete clientThreadList->at(i);
		}

		delete serverThreadList;
		delete clientThreadList;

#elif OS_PLATFORM == PLATFORM_LINUX

		//todo epoll destroy

		free(event);

#elif OS_PLATFORM == PLATFORM_MAC

		//todo kqueue delete

		free(event);

#endif

		//clean worker thread
		for (int i = 0; i < workerThreadCount; i++)
		{
			workerThreadArray[i]->stop();
			delete workerThreadArray[i];
		}

		delete clientList;
		delete serverList;
		delete connectorInfoPool;
		delete timerQueue;

	}

	bool Network::init()
	{
#if OS_PLATFORM == PLATFORM_WINDOWS

		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			ErrorLog("wsaStartup error");
			return false;
		}

		serverThreadList = new Util::MTList<std::thread*>();
		clientThreadList = new Util::MTList<std::thread*>();

#elif OS_PLATFORM == PLATFORM_LINUX

		eventFd = epoll_create(EVENT_BUFFER_SIZE);
		if (eventFd < 0) {
			perror("epoll create error.");
			exit(2);
		}

		event = (struct epoll_event*)malloc(sizeof(struct epoll_event)*EVENT_BUFFER_SIZE);

		memset(&connectEvent, 0, sizeof(struct epoll_event));

#elif OS_PLATFORM == PLATFORM_MAC

		eventFd = kqueue();
		if (eventFd < 0) {
			ErrorLog("kqueue create error.");
			return false;
		}

		event = (struct kevent*)malloc(sizeof(struct kevent)*EVENT_BUFFER_SIZE);

#endif
		clntaddrLen = sizeof(clntaddr);

		lastLoopTime = 0;

		//start network function
		networkThread = std::thread(&Network::start, this);

		DebugLog("NETWORK START");

		return true;
	}


	long Network::getNetworkCurrentTime()
	{
		//get milliseconds
		std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());

		return ms.count();
	}

	bool Network::addConnector(BaseConnector* connector)
	{
		bool isSuccess = false;
		
		if (connector->type == CONNECTOR_TYPE_SERVER)
			isSuccess = addServer((BaseServer*)connector);
		else
			isSuccess = addClient((BaseClient*)connector);

		return isSuccess;
	}

	bool Network::addTimer(Timer *timer)
	{
		timer->startTime = getNetworkCurrentTime();
		timerQueue->push(timer);

		return true;
	}

	bool Network::addServer(BaseServer* server)
	{
		//create tcp socket and listen
		server->connectorInfo->hostId = CreateTCPServerSocket(server->connectorInfo->ip, server->connectorInfo->port);
		if (server->connectorInfo->hostId < 0)
		{
			printf("create sock error");
			return false;
		}

		serverList->push_back(server);

#if OS_PLATFORM == PLATFORM_WINDOWS

		std::thread* serverThread = new std::thread(&Network::windowsServerThread, this, server);

		serverThreadList->push_back(serverThread);

#elif OS_PLATFORM == PLATFORM_LINUX

		connectEvent.events = EPOLLIN;
		connectEvent.data.ptr = (void*)server;
		epoll_ctl(eventFd, EPOLL_CTL_ADD, server->connectorInfo->hostId, &connectEvent);

#elif OS_PLATFORM == PLATFORM_MAC

		EV_SET(&connectEvent, server->fd, EVFILT_READ, EV_ADD, 0, 0, (void*)server);

		if (kevent(eventFd, &connectEvent, 1, NULL, 0, NULL) == -1)
		{
			ErrorLog("kevent init error");
			return false;
		}

#endif
		return true;
	}

	bool Network::addClient(BaseClient* client)
	{
		//create tcp socket and connect
		int hostId = CreateTCPClientSocket(client->connectorInfo->ip, client->connectorInfo->port);
		if (hostId < 0)
		{
			ErrorLog("create sock error");
			return false;
		}

		//send worker thread connecting info
		ConnectorInfo* connectorInfo = client->connectorInfo;

		connectorInfo->hostId = hostId;

		clientList->push_back(client);

#if OS_PLATFORM == PLATFORM_WINDOWS

		std::thread* clientThread = new std::thread(&Network::windowsConnectorInfoThread, this, connectorInfo);

		clientThreadList->push_back(clientThread);

#else

		setSocketOption(hostId);
		
#if OS_PLATFORM == PLATFORM_LINUX

		connectEvent.events = EPOLLIN;
		connectEvent.data.ptr = (void*)connectorInfo;
		epoll_ctl(eventFd, EPOLL_CTL_ADD, connectorInfo->hostId, &connectEvent);

#elif OS_PLATFORM == PLATFORM_MAC

		EV_SET(&connectEvent, connectorInfo->hostId, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, (void*)connectorInfo);

		if (kevent(eventFd, &connectEvent, 1, NULL, 0, NULL) == -1)
		{
			printf("kevent init error");
			return false;
		}

#endif

		WorkerThread* workerThread = getWorkerThreadUsingHash(connectorInfo->hostId);

		DataPacket* dp = workerThread->dataPacketPool->getObject();

		dp->setDataPacket(RECEIVE_TYPE_CONNECT, connectorInfo->hostId, connectorInfo->connector->connectorInfo->hostId);

		workerThread->pushDataPacket(dp);

#endif
		
		return true;
	}


	int Network::CreateTCPServerSocket(const char* ip, unsigned short port)
	{
		int sock;
		struct sockaddr_in servaddr;

		if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		{
			ErrorLog("socket() failed.\n");
			return -1;
		}

		memset(&servaddr, 0, sizeof(struct sockaddr_in));
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = inet_addr(ip);
		servaddr.sin_port = htons(port);

		if (bind(sock, (struct sockaddr*)&servaddr, sizeof(struct sockaddr_in)) < 0)
		{
			ErrorLog("Bind() failed.\n");
			return -2;
		}

		if (listen(sock, SOMAXCONN)) {
			printf("Listen() failed.\n");
			return -3;
		}

		return sock;
	}


	int Network::CreateTCPClientSocket(const char* ip, unsigned short port)
	{
		int sock;
		struct sockaddr_in addr;

		if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		{
			ErrorLog("socket() failed.\n");
			return -1;
		}

		memset(&addr, 0, sizeof(struct sockaddr_in));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(ip);
		addr.sin_port = htons(port);

		if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		{
			ErrorLog("can't connect.\n");
			return -2;
		}

		return sock;
	}

	void Network::sendData(BaseConnector* connector, const char* data, int dataSize)
	{
		sendData(connector->connectorInfo->hostId, data, dataSize);
	}

	void Network::sendData(ConnectorInfo* connectorInfo, const char* data, int dataSize)
	{
		sendData(connectorInfo->hostId, data, dataSize);
	}

	void Network::sendData(int fd, const char* data, int dataSize)
	{
		if (send(fd, data, dataSize, 0) != dataSize)
		{
			ErrorLog("sendMessage error - host id : %d", fd);
		}
		else
		{
			DebugLog("데이터를 발신하였습니다.");
		}
	}
	

	void Network::disconnectWithConnectorInfo(WorkerThread* workerThread, ConnectorInfo* connectorInfo)
	{
		DebugLog("disconnectWithConnectorInfo - hostId : %d", connectorInfo->hostId);

		//disconnect connectorInfo
		int fd = connectorInfo->hostId;

#if OS_PLATFORM == PLATFORM_WINDOWS

		closesocket(fd);

#else

#if OS_PLATFORM == PLATFORM_LINUX

		epoll_ctl(eventFd, EPOLL_CTL_DEL, fd, event);

#elif OS_PLATFORM == PLATFORM_MAC

		EV_SET(&connectEvent, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);


		if (kevent(eventFd, &connectEvent, 1, NULL, 0, NULL) == -1)
		{
			ErrorLog("kevent init error");
		}

#endif

		close(fd);

#endif

		//send disconnecting data packet
		DataPacket* dp = workerThread->dataPacketPool->getObject();

		dp->setDataPacket(RECEIVE_TYPE_DISCONNECT, connectorInfo);

		workerThread->pushDataPacket(dp);
	}

	//start receiving loop
	void Network::start()
	{
		int eventCnt;
		long timeInterval;

		lastLoopTime = getNetworkCurrentTime() - 1;

		while (true)
		{
			currentTime = getNetworkCurrentTime();

			long currentLoopDt = currentTime - lastLoopTime;

			if (currentLoopDt > NETWORK_LOOP_DT)
				currentLoopDt = NETWORK_LOOP_DT;

#if OS_PLATFORM == PLATFORM_WINDOWS
			//start
			if (currentLoopDt < NETWORK_LOOP_DT)
			{
				Sleep(NETWORK_LOOP_DT - currentLoopDt);
			}
			else
			{
				lastLoopTime = currentTime;

				//do loop
			}
		}
	}

#else




#if OS_PLATFORM == PLATFORM_LINUX

			if (currentLoopDt >= NETWORK_LOOP_DT)
			{
				eventCnt = epoll_wait(eventFd, event, EVENT_BUFFER_SIZE, 0);
			}
			else
			{
				eventCnt = epoll_wait(eventFd, event, EVENT_BUFFER_SIZE, NETWORK_LOOP_DT - currentLoopDt);
			}

			lastLoopTime = currentTime;

			if (eventCnt < 0)
			{
				ErrorLog("kevent wait fail.");
				continue;
			}
			else if (eventCnt == 0)
			{
				continue;
			}

			for (int i = 0; i < eventCnt; i++)
			{
				if (true) //if threadEvent == false
				{
					bool isServer = false;
					for (int j = 0; j < serverList->size(); j++)
					{
						Server* server = serverList->at(j);
						if (event[i].data.ptr == server) // Server sock
						{

#elif OS_PLATFORM == PLATFORM_MAC

			struct timespec waitTime;

			if (nextPingCheckTime == LONG_MAX)
			{
				eventCnt = kevent(eventFd, NULL, 0, event, EVENT_BUFFER_SIZE, NULL);
			}
			else
			{
				timeInterval = nextPingCheckTime - currentTime;
				waitTime.tv_sec = timeInterval / 1000;
				waitTime.tv_nsec = (timeInterval % 1000) * 1000000;

				eventCnt = kevent(eventFd, NULL, 0, event, EVENT_BUFFER_SIZE, &waitTime);
			}

			if (eventCnt < 0)
			{
				ErrorLog("kevent wait fail.");
				continue;
			}
			else if (eventCnt == 0)
			{
				continue;
			}

			for (int i = 0; i < eventCnt; i++)
			{
				if (event[i].filter == EVFILT_READ)
				{
					bool isServer = false;

					for (int j = 0; j < listenSocketCount; j++)
					{
						if (event[i].ident == listenerInfoVector.at(j)->fd) // Server sock
						{

#endif

							isServer = true;
							clntFd = accept(server->connectorInfo->hostId, (struct sockaddr *)&clntaddr, (socklen_t*)&clntaddrLen);
							if (clntFd < 0)
							{
								ErrorLog("accept fail.");
								break;
							}

							ConnectorInfo* connectorInfo = connectorInfoPool->getObject();
							connectorInfo->init(server, clntFd);

							setSocketOption(clntFd);

#if OS_PLATFORM == PLATFORM_LINUX

							connectEvent.events = EPOLLIN;
							connectEvent.data.ptr = (void*)connectorInfo;
							epoll_ctl(eventFd, EPOLL_CTL_ADD, connectorInfo->hostId, &connectEvent);

#elif OS_PLATFORM == PLATFORM_MAC

							EV_SET(&connectEvent, clntFd, EVFILT_READ, EV_ADD, 0, 0, (void*)connectorInfo);

							if (kevent(eventFd, &connectEvent, 1, NULL, 0, NULL) == -1)
							{
								ErrorLog("kevent init error");
								break;
							}

#endif
							if (server->connectorInfoMap.insert(std::pair<int, ConnectorInfo*>(connectorInfo->hostId, connectorInfo)).second == false)
							{
								ErrorLog("insert error");
								connectorInfoPool->returnObject(connectorInfo);
								break;
							}
							
							WorkerThread* workerThread = getWorkerThreadUsingHash(connectorInfo->hostId);

							DataPacket* dp = workerThread->dataPacketPool->getObject();

							dp->setDataPacket(RECEIVE_TYPE_CONNECT, connectorInfo->hostId, connectorInfo->connector->connectorInfo->hostId);

							workerThread->pushDataPacket(dp);

							break;
						}
					}

					if (isServer) continue;

					ConnectorInfo* connectorInfo;

#if OS_PLATFORM == PLATFORM_LINUX

					connectorInfo = (ConnectorInfo*)event[i].data.ptr;
					clntFd = connectorInfo->hostId;

#elif OS_PLATFORM == PLATFORM_MAC

					clntFd = (int)event[i].ident;
					connectorInfo = (ConnectorInfo*)event[i].udata;

#endif

					processReceiveData(connectorInfo);
				}
			}
		}
	}

#endif
}