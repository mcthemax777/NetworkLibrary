#ifndef __WORKER_THREAD_H__
#define __WORKER_THREAD_H__

#define HAVE_STRUCT_TIMESPEC

#include <pthread.h>
#include <deque>
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
		WorkerThread();
		~WorkerThread();
		bool initialize();
		void run();

		void pushDataPacket(DataPacket* dataPacket);

		pthread_t* getTid() { return &tid; }
		pthread_cond_t* getCond() { return &cond; }
		int getDataPacketCount();

	protected:
		pthread_t tid;
		pthread_mutex_t mutex;
		pthread_cond_t cond;
		//std::deque<DataPacket*>* dataPacketQueue;

	public:
		Util::BQueue<DataPacket*>* dataPacketQueue;
		Util::ObjectPool<DataPacket>* dataPacketPool;
		Util::ObjectPool<Buffer>* bufferPool;
	};
}
#endif //__WORKER_THREAD_H__


