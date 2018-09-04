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
#include <list>
#include "Util/Singleton.h"
#include "Define.h"
#include "BaseServer.h"
#include "BaseClient.h"
#include "Util/ObjectPool.h"
#include "Util/List/MTList.h"
#include "Util/List/STList.h"
#include "Buffer.h"


#define MAX_IP_LEN 15
#define EVENT_BUFFER_SIZE 50
#define MAX_CONNECT_SIZE 100
#define NETWORK_LOOP_DT 1000

typedef int8_t dataType_t;
typedef uint64_t dataSize_t;

typedef int stringSize_t;

namespace CG
{
	class Timer;
	class ConnectorInfo;
	class BaseConnector;
	class WorkerThread;

	enum RECEIVE_TYPE
	{
		RECEIVE_TYPE_CONNECT = 0,
		RECEIVE_TYPE_DATA,
		RECEIVE_TYPE_DISCONNECT,
	};

	class DataPacket
	{
	public:
		int receiveType;
		ConnectorInfo* connectorInfo;
		Buffer* buffer;

		void setDataPacket(int _receiveType, ConnectorInfo* _connectorInfo, Buffer* _buffer = nullptr)
		{
			receiveType = _receiveType;
			connectorInfo = _connectorInfo;
			buffer = _buffer;
		}

		~DataPacket()
		{

		}
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

	class Network : public Util::Singleton<Network>
	{
	public:
		int workerThreadCount;
		WorkerThread** workerThreadArray;

	public:
		~Network();

		long getNetworkCurrentTime();
		bool init();

		bool addConnector(BaseConnector* connect);
		bool addServer(BaseServer* server);
		bool addClient(BaseClient* client);
		int CreateTCPServerSocket(const char* ip, unsigned short port);
		int CreateTCPClientSocket(const char* ip, unsigned short port);
		bool addTimer(Timer *timer);
		void sendData(int fd, const char* data, int dataSize);
		void sendData(BaseConnector* connector, const char* data, int dataSize);
		void sendData(ConnectorInfo* connectorInfo, const char* data, int dataSize);
		bool processReceiveData(ConnectorInfo* connectorInfo);
		WorkerThread* getWorkerThreadUsingHash(int hashKey);
		void disconnectWithConnectorInfo(WorkerThread* workerThread, ConnectorInfo* connectorInfo);
		void start();
		
		Network();

	public:		
		//나의 Server 정보
		Util::List<BaseServer*>* serverList;
		//나의 Client 정보
		Util::List<BaseClient*>* clientList;
		//Util::List<EventFunction*>* eventFunctionList;
		//등록한 타이머 저장
		Util::Queue<Timer*>* timerQueue;

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
		Util::List<pthread_t*>* serverTidList;
		Util::List<pthread_t*>* clientTidList;

		void windowsConnectorInfoThread(ConnectorInfo* connectorInfo);
		void windowsServerThread(BaseServer* server);

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
	};
}