#include "WorkerThread.h"
#include "Network.h"
#include "Connector.h"

#include "Log/Log.h"
#include <stdlib.h>

namespace CG
{
	WorkerThread::WorkerThread(bool isMultiThread)
	{
		if(isMultiThread)
			dataPacketQueue = new Util::BQueue<DataPacket*>();
		else
			dataPacketQueue = new Util::NBQueue<DataPacket*>();

		dataPacketPool = new Util::ObjectPool<DataPacket>(10, true);
		bufferPool = new Util::ObjectPool<Buffer>(10, true);
	}


	WorkerThread::~WorkerThread()
	{

	}

	bool WorkerThread::initialize()
	{
		return true;
	}


	void WorkerThread::run()
	{
		while (true)
		{
			DataPacket* dp = dataPacketQueue->pop();
			if (dp != NULL)
			{
				EventFunction* eventFunction = Network::GetInstance()->getEventFunction(dp->eventFunctionHostId);

				if (eventFunction == nullptr)
				{
					ErrorLog("fucking man");
				}
				else
				{
					if (dp->receiveType == RECEIVE_TYPE_DATA)
					{
						Connector* c = dp-> 
						eventFunction->onReceive(dp->hostId, dp->buffer->data + dp->startIndex, dp->dataSize);

						if(dp->isLastDataInBuffer)
							bufferPool->returnObject(dp->buffer);
					}
					else if (dp->receiveType == RECEIVE_TYPE_CONNECT)
					{
						eventFunction->onConnect(dp->hostId);
					}
					else if (dp->receiveType == RECEIVE_TYPE_DISCONNECT)
					{
						eventFunction->onDisconnect(dp->hostId);
					}
					else
					{
						ErrorLog("invalid receive type - %d", dp->receiveType);
					}
				}

				dataPacketPool->returnObject(dp);
			}
			else
			{
				if (Network::GetInstance()->workerThreadCount != 0)
				{
					ErrorLog("not wait queue");
				}
				else
				{

				}
				break;
			}
		}
	}

	void WorkerThread::pushDataPacket(DataPacket* dataPacket)
	{
		dataPacketQueue->push(dataPacket);
	}

	int WorkerThread::getDataPacketCount()
	{
		return (int)dataPacketQueue->size();
	}
}