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
#include "Server.h"
#include "Client.h"
#include "Util/ObjectPool.h"
#include "Util/List/MTList.h"
#include "Util/List/STList.h"
#include "CGDataConvertor.h"


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
	class Connector;
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
		HostId hostId;
		HostId eventFunctionHostId;
		Buffer* buffer;
		int startIndex;
		int dataSize;
		bool isLastDataInBuffer;

		void setDataPacket(int _receiveType, HostId _hostId, HostId _eventFunctionHostId, Buffer* _buffer = nullptr, int _startIndex = 0, int _dataSize = 0, bool _isLastDataInBuffer = true)
		{
			receiveType = _receiveType;
			hostId = _hostId;
			eventFunctionHostId = _eventFunctionHostId;
			buffer = _buffer;
			startIndex = _startIndex;
			dataSize = _dataSize;
			isLastDataInBuffer = _isLastDataInBuffer;
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
		bool addTimer(Timer *timer);
		void sendData(int fd, const char* data, int dataSize);
		void sendMessage(Connector* connector, HostId hostId, const char* data, int dataSize);
		void sendMessage(Connector* connector, const char* data, int dataSize);
		void sendMessage(ConnectorInfo* connectorInfo, const char* data, int dataSize);
		void sendDataToWorkerThreadWithConverting(WorkerThread* workerThread, ConnectorInfo* connectorInfo, Buffer* buffer);
		bool processReceiveData(ConnectorInfo* connectorInfo);
		WorkerThread* getWorkerThreadUsingHash(int hashKey);
		void disconnectWithConnectorInfo(WorkerThread* workerThread, ConnectorInfo* connectorInfo);
		EventFunction* getEventFunction(HostId hostId);
		bool removeEventFunction(EventFunction* eventFuction);
		void start();
		
		Network();

	public:		
		//나의 Server 정보
		Util::List<Server*>* serverList;
		//나의 Client 정보
		Util::List<Client*>* clientList;
		Util::List<EventFunction*>* eventFunctionList;
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
	};
}