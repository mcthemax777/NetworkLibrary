#pragma once

#define HAVE_STRUCT_TIMESPEC

#include <pthread.h>
#include <deque>
#include <list>
#include "Util/Queue/BQueue.h"
#include "Util/ObjectPool.h"


namespace CG
{
	class ConnectorInfo;
	class DataPacket;
	class Buffer;

	class WorkerThread
	{
	public:
		WorkerThread(bool isMultiThread);
		~WorkerThread();
		bool initialize();
		void run();

		void pushDataPacket(DataPacket* dataPacket);

		pthread_t* getTid() { return &tid; }
		int getDataPacketCount();

	protected:
		pthread_t tid;

	public:
		Util::Queue<DataPacket*>* dataPacketQueue;
		Util::ObjectPool<DataPacket>* dataPacketPool;
		Util::ObjectPool<Buffer>* bufferPool;

		std::list<Buffer*>::iterator itr;
	};
}
