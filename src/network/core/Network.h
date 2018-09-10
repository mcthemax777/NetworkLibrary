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

///atoi() is desperate but i want to use to clean code (if don't use, divide linux and windows)
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

	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief received type that set in dataPacket
	* @todo none
	*/
	enum RECEIVE_TYPE
	{
		RECEIVE_TYPE_CONNECT = 0,
		RECEIVE_TYPE_DATA,
		RECEIVE_TYPE_DISCONNECT,
	};

	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief When request come, network thread send this to worker thread 
	* @todo thinking divide DataPacket(connect, disconnet, receive)
	*/
	class DataPacket
	{
	private:
		///enum type
		int receiveType;
		
		/// requesting user info
		ConnectorInfo* connectorInfo;
		
		/// received data (null when connect and disconnet)
		Buffer* buffer;

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief Set data in this
		* @todo none
		*/
		void setDataPacket(int _receiveType, ConnectorInfo* _connectorInfo, Buffer* _buffer = nullptr)
		{
			receiveType = _receiveType;
			connectorInfo = _connectorInfo;
			buffer = _buffer;
		}

	public:
		friend class Network;
		friend class WorkerThread;
	};

	//it is divided dataPacket (now, not use) 
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


	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief whole network function is in this class
	* @todo none
	*/
	class Network : public Util::Singleton<Network>
	{
	public:
		///worker thread count
		int workerThreadCount;

		///worker thread array(storage pointer array)
		WorkerThread** workerThreadArray;

	public:
		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief constructure. call init function(because this class is singleton)
		*/
		Network();

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief destructure. delete and clean whole member value
		*/
		~Network();

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief get current time.
		*/
		long getNetworkCurrentTime();

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief init all network function to ready network commuication
		* @return bool result : is success init
		*/
		bool init();

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief set connector to using network function
		* @parameter BaseConnector* connector : to regist connector
		* @return bool result : is success add connector
		*/
		bool addConnector(BaseConnector* connector);

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief set server to using network function
		* @parameter BaseServer* server : to regist server
		* @return bool result : is success add server
		*/
		bool addServer(BaseServer* server);

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief set client to using network function
		* @parameter BaseClient* client : to regist client
		* @return bool result : is success add client
		*/
		bool addClient(BaseClient* client);

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief create tcp server socket and listen to receive client
		* @parameter const char* ip : open ip
		* @parameter unsigned short port : open port
		* @return int result : if success, return file descrpiter. or not, return negative number
		*/
		int CreateTCPServerSocket(const char* ip, unsigned short port);

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief create tcp client socket and connect with server
		* @parameter const char* ip : open ip
		* @parameter unsigned short port : open port
		* @return int result : if success, return file descrpiter. or not, return negative number
		*/
		int CreateTCPClientSocket(const char* ip, unsigned short port);

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief add timer
		* @return bool result : if success true
		*/
		bool addTimer(Timer *timer);

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief send data to receiving fd
		* @parameter int fd : file descript
		* @parameter const char* data : data to send
		* @parameter int dataSize : data size
		*/
		void sendData(int fd, const char* data, int dataSize);

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief send data to receiving connector
		* @parameter BaseConnector* connector : connector delivering data
		* @parameter const char* data : data to send
		* @parameter int dataSize : data size
		*/
		void sendData(BaseConnector* connector, const char* data, int dataSize);

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief send data to receiving connectorInfo
		* @parameter ConnectorInfo* connectorInfo : connector info delivering data
		* @parameter const char* data : data to send
		* @parameter int dataSize : data size
		*/
		void sendData(ConnectorInfo* connectorInfo, const char* data, int dataSize);

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief choose worker thread and send data to worker thread
		* @parameter ConnectorInfo* connectorInfo : connect info that send data
		* @return bool result : if receive request, true. if disconnect request or error, false
		* @todo process if receive error
		*/
		bool processReceiveData(ConnectorInfo* connectorInfo);

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief decision worker thread that will process task 
		* @parameter int hashKey : now just file descript
		* @return WorkerThread* result : choising worker thread
		*/
		WorkerThread* getWorkerThreadUsingHash(int hashKey);

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief process disconnect
		* @parameter WorkerThread* workerThread : worker thread that will process task
		* @parameter ConnectorInfo* connectorInfo : connector info who is disconnected
		*/
		void disconnectWithConnectorInfo(WorkerThread* workerThread, ConnectorInfo* connectorInfo);

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief start all network task while(true)
		*/
		void start();
		
	public:		
		/// storage creating server
		Util::List<BaseServer*>* serverList;
		/// storage creating client
		Util::List<BaseClient*>* clientList;
		/// storage creating timer
		Util::Queue<Timer*>* timerQueue;

		///connectorInfo pool to reduce allocate action
		Util::ObjectPool<ConnectorInfo>* connectorInfoPool;

		//explaiin in cpp
		struct sockaddr_in clntaddr;
		int clntaddrLen;

		long currentTime;
		long lastLoopTime;

		long loopDt;

		std::thread networkThread;


#if OS_PLATFORM == PLATFORM_WINDOWS
	private:
		WSAData wsaData;

	public:

		//to manage thread list(if change iocp, remove)
		Util::List<std::thread*>* serverThreadList;
		Util::List<std::thread*>* clientThreadList;
		
		///create client thread that wait client action
		void windowsConnectorInfoThread(ConnectorInfo* connectorInfo);
		
		///create server thread to listen
		void windowsServerThread(BaseServer* server);

#else
		int eventFd;
		int clntFd;

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