#include "Network.h"

#if OS_PLATFORM == PLATFORM_WINDOWS

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <windows.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

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
#include "Log/Log.h"
#include "Connector.h"
#include "ConnectorInfo.h"
#include "Define.h"
#include "Server.h"
#include "Client.h"
#include "Timer.h"
#include "DataConvertor.h"




#if OS_PLATFORM == PLATFORM_WINDOWS
#include < time.h >
#include <WinSock2.h>

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

struct timezone
{
	int  tz_minuteswest; /* minutes W of Greenwich */
	int  tz_dsttime;     /* type of dst correction */
};


int gettimeofday(struct timeval *tv, struct timezone *tz)
{
	FILETIME ft;
	unsigned __int64 tmpres = 0;
	static int tzflag = 0;

	if (NULL != tv)
	{
		GetSystemTimeAsFileTime(&ft);

		tmpres |= ft.dwHighDateTime;
		tmpres <<= 32;
		tmpres |= ft.dwLowDateTime;

		tmpres /= 10;  /*convert into microseconds*/
					   /*converting file time to unix epoch*/
		tmpres -= DELTA_EPOCH_IN_MICROSECS;
		tv->tv_sec = (long)(tmpres / 1000000UL);
		tv->tv_usec = (long)(tmpres % 1000000UL);
	}

	if (NULL != tz)
	{
		if (!tzflag)
		{
			_tzset();
			tzflag++;

		}
		tz->tz_minuteswest = _timezone / 60;
		tz->tz_dsttime = _daylight;
	}
	return 0;
}

#endif



namespace CG
{
	WorkerThread* Network::getWorkerThreadUsingHash(int hashKey)
	{
		int hash = hashKey % workerThreadCount;

		return workerThreadArray[hash];
	}

	void Network::sendDataToWorkerThreadWithConverting(WorkerThread* workerThread, ConnectorInfo* connectorInfo, Buffer* buffer)
	{
		char* currentData = buffer->data;

		int currentDataSize = buffer->dataSize;

		int currentBufferIndex = 0;

		while (true)
		{
			int index = connectorInfo->dataConvertor->convert(currentData, currentDataSize);


			if (index == 0) //불완전한 형태의 데이터일때
			{
				if (buffer->data != currentData) // currentdata가 가르키는 위치가 버퍼데이터랑 다르면 리셋 시켜줘야됨.
				{
					memmove(buffer->data, currentData, currentDataSize);
				}

				buffer->dataSize = currentDataSize;

				connectorInfo->buffer = buffer;

				break;
			}
			if (index < 0)
			{
				ErrorLog("index error - %d", index);
				break;
			}
			else
			{
				currentData += index;
				currentDataSize -= index;

				DataPacket* dp = workerThread->dataPacketPool->getObject();

				if (currentDataSize == 0)
				{
					dp->setDataPacket(RECEIVE_TYPE_DATA, connectorInfo->hostId, connectorInfo->connector->connectorInfo->hostId, buffer, currentBufferIndex, index, true);

					workerThread->pushDataPacket(dp);

					connectorInfo->buffer = nullptr;
				
					break;
				}
				else
				{
					dp->setDataPacket(RECEIVE_TYPE_DATA, connectorInfo->hostId, connectorInfo->connector->connectorInfo->hostId, buffer, currentBufferIndex, index, false);

					workerThread->pushDataPacket(dp);
				}

				currentBufferIndex += index;
			}
		}
	}

	bool Network::processReceiveData(ConnectorInfo* connectorInfo)
	{
		WorkerThread* workerThread = getWorkerThreadUsingHash(connectorInfo->hostId);

		char* receiveDataStartingPoint;

		Buffer* buffer = connectorInfo->buffer;
		
		int receivedDataSize = 0;

		if (buffer == nullptr)
		{
			buffer = workerThread->bufferPool->getObject();

			buffer->dataSize = 0;

			receiveDataStartingPoint = buffer->data;

			connectorInfo->buffer = buffer;
		}
		else
		{
			receivedDataSize = buffer->dataSize;
			receiveDataStartingPoint = buffer->data + receivedDataSize;
		}

		int dataSize = recv(connectorInfo->getHostId(), receiveDataStartingPoint, RECV_BUF - receivedDataSize, 0);

		if (dataSize > 0)
		{
			buffer->dataSize = receivedDataSize + dataSize;

			sendDataToWorkerThreadWithConverting(workerThread, connectorInfo, buffer);

			return true;
		}
		else
		{
			workerThread->bufferPool->returnObject(buffer);
			connectorInfo->buffer = nullptr;

			disconnectWithConnectorInfo(workerThread, connectorInfo);

			if (dataSize != -1)
			{
				ErrorLog("receive count - %d", dataSize);
			}

			return false;
		}
	}

#if OS_PLATFORM == PLATFORM_WINDOWS

	void Network::windowsConnectorInfoThread(ConnectorInfo* connectorInfo)
	{
		WorkerThread* workerThread = getWorkerThreadUsingHash(connectorInfo->hostId);

		DataPacket* dp = workerThread->dataPacketPool->getObject();

		dp->setDataPacket(RECEIVE_TYPE_CONNECT, connectorInfo->hostId, connectorInfo->connector->connectorInfo->hostId);

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

	void Network::windowsServerThread(Server* server)
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

			//ToDo. 쓰레드로 리시브 걸어두고 받으면 워커쓰레드로 던져주기
			pthread_t* tid = new pthread_t();


			if (pthread_create(tid, NULL, Network::GetInstance()->windowsConnectorInfoThread, connectorInfo) != 0)
			{
				ErrorLog("thread create error : ");
				return ;
			}

			clientTidList->push_back(tid);
		}
	}

	void* Network::windowsConnectorInfoThread(void* voidConnectorInfo)
	{
		ConnectorInfo* connectorInfo = (ConnectorInfo*)voidConnectorInfo;

		Network::GetInstance()->windowsConnectorInfoThread(connectorInfo);

		return NULL;
	}

	void* Network::windowsServerThread(void* voidServer)
	{
		Server* server = (Server*)voidServer;

		Network::GetInstance()->windowsServerThread(server);

		return NULL;
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
	void* Network::startUnixRunningThread(void* a)
	{
		Network::GetInstance()->start();

		return NULL;
	}


	void* WorkerThreadFunction(void *data)
	{
		WorkerThread* wt = (WorkerThread*)data;

		DebugLog("run workerThread");

		wt->run();

		return NULL;
	}

	Network::Network()
	{
		workerThreadCount = 4;

		workerThreadArray = new WorkerThread*[workerThreadCount];

		for (int i = 0; i < workerThreadCount; i++)
		{
			workerThreadArray[i] = new WorkerThread();

			if (workerThreadArray[i]->initialize() == false)
			{
				ErrorLog("worker thread error");
				return;
			}

			if (pthread_create(workerThreadArray[i]->getTid(), NULL, WorkerThreadFunction, (void*)(workerThreadArray[i])) != 0)
			{
				ErrorLog("thread create error : ");
				return;
			}
		}

		clientList = new Util::MTList<Client*>();
		serverList = new Util::MTList<Server*>();
		eventFunctionList = new Util::MTList<EventFunction*>();
		timerQueue = new Util::NBQueue<Timer*>();

#if OS_PLATFORM == PLATFORM_WINDOWS
		connectorInfoPool = new Util::ObjectPool<ConnectorInfo>(100, true);
#else
		connectorInfoPool = new Util::ObjectPool<ConnectorInfo>(100, false);
#endif

		init();
	}

	Network::~Network()
	{

#if OS_PLATFORM == PLATFORM_WINDOWS

		WSACleanup();

		for (int i = 0; i < serverTidList->size(); i++)
		{
			pthread_cancel(*(serverTidList->at(i)));
		}

		for (int i = 0; i < clientTidList->size(); i++)
		{
			pthread_cancel(*(clientTidList->at(i)));
		}

		delete serverTidList;
		delete clientTidList;

#elif OS_PLATFORM == PLATFORM_LINUX

		//todo epoll destroy

		free(event);

#elif OS_PLATFORM == PLATFORM_MAC

		//todo kqueue delete

		free(event);

#endif

		for (int i = 0; i < workerThreadCount; i++)
		{
			pthread_cancel(*(workerThreadArray[i]->getTid()));
			delete workerThreadArray[i];
		}

		delete clientList;
		delete serverList;
		delete connectorInfoPool;
		delete eventFunctionList;
		delete timerQueue;

		pthread_cancel(*networkTid);
	}

	bool Network::init()
	{
#if OS_PLATFORM == PLATFORM_WINDOWS

		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			ErrorLog("wsaStartup error");
			return false;
		}

		serverTidList = new Util::MTList<pthread_t*>();
		clientTidList = new Util::MTList<pthread_t*>();

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

		networkTid = new pthread_t();

		if (pthread_create(networkTid, NULL, startUnixRunningThread, NULL) != 0)
		{
			ErrorLog("thread create error : ");
			return false;
		}

		return true;
	}


	long Network::getNetworkCurrentTime()
	{
		struct timeval tv;
		gettimeofday(&tv, NULL);

		return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);    // milliseconds 단위로 변환
	}

	bool Network::addConnector(Connector* connector)
	{
		bool isSuccess = false;
		
		if (connector->type == CONNECTOR_TYPE_SERVER)
			isSuccess = addServer((Server*)connector);
		else
			isSuccess = addClient((Client*)connector);
		
		if (isSuccess)
		{
			EventFunction* eventFunction = new EventFunction();
			eventFunction->hostId = connector->connectorInfo->hostId;
			eventFunction->onConnect = connector->onConnect;
			eventFunction->onReceive = connector->onReceive;
			eventFunction->onDisconnect = connector->onDisconnect;
			eventFunction->dataConvertor = connector->dataConvertor;

			//이미 있는 호스트 아이디인지는 확인해야된다.
			eventFunctionList->push_back(eventFunction);
		}

		return isSuccess;
	}

	bool Network::addTimer(Timer *timer)
	{
		timer->startTime = getNetworkCurrentTime();
		timerQueue->push(timer);

		return true;
	}

	bool Network::addServer(Server* server)
	{
		server->connectorInfo->hostId = CreateTCPServerSocket(server->connectorInfo->ip, server->connectorInfo->port);
		if (server->connectorInfo->hostId < 0)
		{
			printf("create sock error");
			return false;
		}

		serverList->push_back(server);

#if OS_PLATFORM == PLATFORM_WINDOWS

		//ToDo. 쓰레드로 엑셉트해두고 받으면 워커쓰레드로 던져주기
		pthread_t* tid = new pthread_t();


		if (pthread_create(tid, NULL, Network::GetInstance()->windowsServerThread, server) != 0)
		{
			ErrorLog("thread create error");
			return false;
		}

		serverTidList->push_back(tid);

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


	bool Network::addClient(Client* client)
	{
		int hostId = CreateTCPClientSocket(client->connectorInfo->ip, client->connectorInfo->port);
		if (hostId < 0)
		{
			ErrorLog("create sock error");
			return false;
		}

		ConnectorInfo* connectorInfo = client->connectorInfo;

		connectorInfo->hostId = hostId;

		clientList->push_back(client);

#if OS_PLATFORM == PLATFORM_WINDOWS

		//ToDo. 쓰레드로 리시브 걸어두고 받으면 워커쓰레드로 던져주기
		pthread_t* tid = new pthread_t();


		if (pthread_create(tid, NULL, Network::GetInstance()->windowsConnectorInfoThread, connectorInfo) != 0)
		{
			ErrorLog("thread create error : ");
			return false;
		}

		clientTidList->push_back(tid);

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

	EventFunction* Network::getEventFunction(HostId hostId)
	{
		for (int i = 0; i < eventFunctionList->size(); i++)
		{
			EventFunction* eventFunction = eventFunctionList->at(i);
			if (eventFunction->hostId == hostId)
			{
				return eventFunction;
			}
		}
		
		return nullptr;
	}

	bool Network::removeEventFunction(EventFunction* eventFuction)
	{
		/*std::vector<EventFunction*>::iterator itr;
		for (itr = eventFunctionList->begin(); itr != eventFunctionList->end(); itr++)
		{
			if ((*itr)->hostId == hostId)
			{
				EventFunction* eventFunction = *itr;
				eventFunctionList->erase(itr);
				return true;
			}
		}*/

		if (eventFunctionList->remove(eventFuction))
			return true;
		else
			return false;
	}

	void Network::sendMessage(HostId hostId, const char* data, int dataSize)
	{
		if (send(hostId, data, dataSize, 0) != dataSize)
		{
			ErrorLog("sendMessage error");
		}
		else
		{
			DebugLog("데이터를 발신하였습니다.");
		}
	}
	

	void Network::disconnectWithConnectorInfo(WorkerThread* workerThread, ConnectorInfo* connectorInfo)
	{
		DebugLog("disconnectWithConnectorInfo - hostId : %d", connectorInfo->hostId);

		DataPacket* dp = workerThread->dataPacketPool->getObject();

		dp->setDataPacket(RECEIVE_TYPE_DISCONNECT, connectorInfo->hostId, connectorInfo->connector->connectorInfo->hostId);

		workerThread->pushDataPacket(dp);

		//connectorInfo 연결 끊기
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

		if (connectorInfo->connector->getConnectorType() == CONNECTOR_TYPE_SERVER)
		{
			if (((Server*)connectorInfo->connector)->connectorInfoMap.erase(connectorInfo->hostId) <= 0)
				ErrorLog("already remove connectorInfo");
		}
		else
		{
			Client* client = ((Client*)connectorInfo->connector);
			client->connectorInfo = nullptr;
		}

		connectorInfo->reset();

		connectorInfoPool->returnObject(connectorInfo);
	}

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

#elif OS_PLATFORM == PLATFORM_WINDOWS




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