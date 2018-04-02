#pragma once

#include "SystemDefine.h"


#if OS_PLATFORM == PLATFORM_LINUX
#include <sys/epoll.h>
#include <stdint.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/time.h>
#include <cstring>

#elif OS_PLATFORM == PLATFORM_WINDOWS

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#define HAVE_STRUCT_TIMESPEC
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>

#elif OS_PLATFORM == PLATFORM_MAC
#include <sys/event.h>
#include <stdint.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/time.h>
#include <cstring>

#endif

#include <functional>
#include <vector>
#include <pthread.h>
#include "Util/Singleton.h"
#include "Define.h"
#include "DataConvertor.h"
#include "Server.h"
#include "Client.h"
#include "Util/ObjectPool.h"

#define MAX_IP_LEN 15
#define EVENT_BUFFER_SIZE 50
#define MAX_CONNECT_SIZE 100
#define RECV_BUF 5000
#define NETWORK_LOOP_DT 1000

typedef int8_t dataType_t;
typedef uint64_t dataSize_t;

typedef int stringSize_t;

namespace NetworkFramework
{
	class Timer;
	class ConnectorInfo;
	class Connector;
	class WorkerThread;

	enum RECEIVE_TYPE
	{
		RECEIVE_TYPE_CONNECT = 0,
		RECEIVE_TYPE_DATA,
		RECEIVE_TYPE_DISCONNECT,
	};

	class Buffer
	{
	public:
		char buffer[RECV_BUF];

		void reset() { memcpy(buffer, 0, RECV_BUF); }
	};

	class DataPacket
	{
	public:
		int receiveType;
		HostId hostId;
		HostId eventFunctionHostId;
		char* data;
		dataSize_t dataSize;
	};

	//이거 나중에 메모리풀 3개로 나눠야되서 귀찮아지네.. 그냥 안씀
	class ConnectDataPacket : public DataPacket
	{
	public:
		ConnectDataPacket() { receiveType = RECEIVE_TYPE_CONNECT; }
	};

	class ReceiveDataPacket : public DataPacket
	{
	public:
		ReceiveDataPacket() { receiveType = RECEIVE_TYPE_DATA; }

		char* data;
		dataSize_t dataSize;
	};

	class DisconnectDataPacket : public DataPacket
	{
	public:
		DisconnectDataPacket() { receiveType = RECEIVE_TYPE_DISCONNECT; }
	};

	class EventFunction
	{
	public:
		HostId hostId;
	public:
		std::function<void(HostId)> onConnect;
		std::function<void(HostId)> onDisconnect;
		std::function<void(HostId, char*, int)> onReceive;
		DataConvertor* dataConvertor;
	};

	class Network : public Util::Singleton<Network>
	{
	public:
		int workerThreadCount;
		WorkerThread** workerThreadArray;

	public:
		~Network();

		long getNetworkCurrentTime();
		bool init();

		bool addConnector(Connector* connect);
		bool addServer(Server* server);
		bool addClient(Client* client);
		int CreateTCPServerSocket(const char* ip, unsigned short port);
		int CreateTCPClientSocket(const char* ip, unsigned short port);
		void sendDataToWorkerThread(int receiveType, ConnectorInfo* const connectorInfo, char* data, int dataSize);
		bool addTimer(Timer *timer);
		void sendMessage(HostId hostId, const char* data, int dataSize);
		void sendDataToWorkerThreadWithConverting(ConnectorInfo* connectorInfo, char* data, int dataSize);
		bool processReceiveData(ConnectorInfo* connectorInfo, char* receiveBuffer, int bufferSize);
		void initConnectorInfo(ConnectorInfo* connectorInfo, Connector* connector, HostId hostId);
		void resetConnectorInfo(ConnectorInfo* connectorInfo);

		void disconnectWithConnectorInfo(ConnectorInfo* connectorInfo);
		EventFunction* getEventFunction(HostId hostId);
		bool removeEventFunction(HostId hostId);
		void start();
		
		Network();

	public:
		char recvBuffer[RCV_BUF];
		
		//나의 Server 정보
		std::vector<Server*>* serverList;
		//나의 Client 정보
		std::vector<Client*>* clientList;
		std::vector<EventFunction*>* eventFunctionList;
		//등록한 타이머 저장
		std::vector<Timer*> timerQueue;

		int eventFd;
		int clntFd;
		struct sockaddr_in clntaddr;
		int clntaddrLen;

		long currentTime;
		long lastLoopTime;

		long loopDt;

		pthread_t* networkTid;
		static void* startUnixRunningThread(void* a);


#if OS_PLATFORM == PLATFORM_WINDOWS
		WSAData wsaData;

	public:
		std::vector<pthread_t*>* serverTidList;
		std::vector<pthread_t*>* clientTidList;

		void windowsConnectorInfoThread(ConnectorInfo* connectorInfo);
		void windowsServerThread(Server* server);

		static void* windowsServerThread(void* voidServer);
		static void* windowsConnectorInfoThread(void* voidConnectorInfo);
#else
		void setSocketOption(int fd);
#if OS_PLATFORM == PLATFORM_LINUX
		struct epoll_event* event;
		struct epoll_event connectEvent;
#elif OS_PLATFORM == PLATFORM_MAC
	private:
		struct kevent* event;
		struct kevent connectEvent;
#endif

#endif
		long nextPingCheckTime;

		Util::ObjectPool<ConnectorInfo>* connectorInfoPool;
		Util::ObjectPool<Buffer>* bufferPool;
	};
}