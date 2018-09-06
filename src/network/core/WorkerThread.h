#pragma once

#define HAVE_STRUCT_TIMESPEC

#include "util/thread/Thread.h"
#include <deque>
#include <list>
#include "Util/Queue/BQueue.h"
#include "Util/ObjectPool.h"


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
