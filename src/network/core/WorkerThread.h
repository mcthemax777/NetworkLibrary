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

	class WorkerThread : public Util::Thread
	{
	public:
		WorkerThread(bool isMultiThread);
		~WorkerThread();
		bool initialize();
		void run();

		void pushDataPacket(DataPacket* dataPacket);

		int getDataPacketCount();

	protected:

	public:
		Util::Queue<DataPacket*>* dataPacketQueue;
		Util::ObjectPool<DataPacket>* dataPacketPool;
		Util::ObjectPool<Buffer>* bufferPool;

		std::list<Buffer*>::iterator itr;

	};
}
