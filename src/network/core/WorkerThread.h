#pragma once

#include "util/thread/Thread.h"
#include <deque>
#include <list>
#include "util/queue/BQueue.h"
#include "util/objectPool/ObjectPool.h"


namespace CG
{
	class ConnectorInfo;
	class DataPacket;
	class Buffer;

	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief process whole request
	*/
	class WorkerThread : public Util::Thread
	{
	public:

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief allocate member value
		* @param bool isMultiThread : if worker_thread_count bigger more than 1, return true 
		*/
		WorkerThread(bool isMultiThread);
		~WorkerThread();

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief init (now no action)
		*/
		bool initialize();

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief wait data packet. if data packet is not coming, sleep until coming
		*/
		void run();

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief network thread use it. push data packet
		*/
		void pushDataPacket(DataPacket* dataPacket);

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief data packet count in queue
		*/
		int getDataPacketCount();

	protected:

	public:
		///data packet queue
		Util::Queue<DataPacket*>* dataPacketQueue;
		
		///data packet pool
		Util::ObjectPool<DataPacket>* dataPacketPool;
		
		///buffer pool
		Util::ObjectPool<Buffer>* bufferPool;

		///to use in functions
		std::list<Buffer*>::iterator itr;

	};
}
