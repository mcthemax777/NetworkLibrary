#pragma once

#include "SystemDefine.h"
#include "NetworkDefine.h"

#if OS_PLATFORM == PLATFORM_LINUX
#include <sys/epoll.h>
#include <stdint.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/time.h>
#include <cstring>

#elif OS_PLATFORM == PLATFORM_WINDOWS

///atoi is desperate but i want to use to clean code (if don't use, divide linux and windows)
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
#include <thread>
#include <list>
#include "util/singleton/Singleton.h"
#include "BaseServer.h"
#include "BaseClient.h"
#include "util/objectPool/ObjectPool.h"
#include "util/List/MTList.h"
#include "util/List/STList.h"
#include "Buffer.h"


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
	private:
		int receiveType;
		ConnectorInfo* connectorInfo;
		Buffer* buffer;

		void setDataPacket(int _receiveType, ConnectorInfo* _connectorInfo, Buffer* _buffer = nullptr)
		{
			receiveType = _receiveType;
			connectorInfo = _connectorInfo;
			buffer = _buffer;
		}

	public:
		friend class Network;
	};

	//이거 나중에 메모리풀 3개로 나눠야되서 귀찮아지네.. 그냥 안씀
	/*class ConnectDataPacket : public DataPacket
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
	};*/

	class Network : public Util::Singleton<Network>
	{
	public:
		int workerThreadCount;
		WorkerThread** workerThreadArray;

	public:
		Network();
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
		
	public:		
		//나의 Server 정보
		Util::List<BaseServer*>* serverList;
		//나의 Client 정보
		Util::List<BaseClient*>* clientList;
		//등록한 타이머 저장
		Util::Queue<Timer*>* timerQueue;

		int eventFd;
		int clntFd;
		struct sockaddr_in clntaddr;
		int clntaddrLen;

		long currentTime;
		long lastLoopTime;

		long loopDt;

		std::thread networkThread;
		static void* startUnixRunningThread(void* a);


#if OS_PLATFORM == PLATFORM_WINDOWS
	private:
		WSAData wsaData;

	public:
		Util::List<std::thread*>* serverThreadList;
		Util::List<std::thread*>* clientThreadList;
		Util::ObjectPool<ConnectorInfo>* connectorInfoPool;

		void windowsConnectorInfoThread(ConnectorInfo* connectorInfo);
		void windowsServerThread(BaseServer* server);

#else

		void setSocketOption(int fd);

#if OS_PLATFORM == PLATFORM_LINUX
	private:
		struct epoll_event* event;
		struct epoll_event connectEvent;

#elif OS_PLATFORM == PLATFORM_MAC

	private:
		struct kevent* event;
		struct kevent connectEvent;
#endif

#endif
	};
}