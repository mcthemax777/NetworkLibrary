#pragma once

#include <functional>
#include <list>
#include "NetworkDefine.h"

namespace CG
{
	class Buffer;
	class BaseConnector;
	class DataConvertor;

	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief if someone connect with me, create this. this class include connector info
	*/
	class ConnectorInfo
	{
	public:
		HostId getHostId() { return hostId; }

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief send data to server or client
		* @param char* data : receive data from other
		* @param int dataSize : receive data size
		*/
		void sendData(const char* data, int dataSize);

	protected:
		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief set connector info and link with connector class
		* @param BaseConnector* _connector : class that made me
		* @param HostId _hostId : socket descripter
		*/
		void init(BaseConnector* _connector, HostId _hostId);

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief reset all contents
		*/
		void reset();

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief storage receive data if the data came partially
		*/
		std::list<Buffer*> bufferList;

		char* ip;
		unsigned short port;
		HostId hostId;

		BaseConnector* connector; ///< set this class who create me

	public:
		friend class BaseConnector;
		friend class Network;
		friend class WorkerThread;
	};
}