#include "WorkerThread.h"
#include "Network.h"
#include "BaseConnector.h"

#include "util/log/Log.h"
#include <cstdlib>

namespace CG
{
	WorkerThread::WorkerThread(bool isMultiThread)
	{
		if(isMultiThread) //multi
			dataPacketQueue = new Util::BQueue<DataPacket*>();
		else //single
			dataPacketQueue = new Util::NBQueue<DataPacket*>();

		dataPacketQueue = new Util::BQueue<DataPacket*>();
		dataPacketPool = new Util::ObjectPool<DataPacket>(100, true);
		bufferPool = new Util::ObjectPool<Buffer>(100, true);
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
		while (!isStop)
		{
			DataPacket* dp = dataPacketQueue->pop();
			if (dp != NULL)
			{
				DebugLog("received data packet - data packet address : %p", dp);

				ConnectorInfo* connectorInfo = dp->connectorInfo;
				BaseConnector* connector = connectorInfo->connector;
				std::list<Buffer*>* bufferList = &(connectorInfo->bufferList);
				
				if (dp->receiveType == RECEIVE_TYPE_DATA)
				{
					DebugLog("current buffer size - %d", bufferList->size());

					//���� ���۸� ���� ó������ ���� ���۸���Ʈ�� �߰�
					connectorInfo->bufferList.push_back(dp->buffer);

					int dataSize = 0;

					//���ۿ� ���� ������ ũ���� �迭 �����
					for (Buffer* buffer : *bufferList)
					{
						dataSize += buffer->dataSize;
					}

					char* data = new char[dataSize];

					//�����͸� ī��.
					int copySize = 0;
					for (Buffer* buffer : *bufferList)
					{
						memcpy(data + copySize, buffer->data + buffer->startIndex, buffer->dataSize);
						copySize += buffer->dataSize;
					}

					//���� �����͸� ����ڰ� ����Ѵ�� ��ȯ�Ͽ� ����
					int usingDataSize = connector->processData(connectorInfo, data, dataSize);

					DebugLog("processing data size - %d", usingDataSize);

					//�����Ҵ��� data�� ����
					delete data;

					if (usingDataSize == 0) //�����Ͱ� �� �ͼ� �׳� �ƹ��͵� ����.
					{

					}
					else
					{
						//�����ϰ� �ִ� ���۵��� ����� ���� ���۴� Ǯ�� �ٽ� ����
						int finishDataSize = usingDataSize;

						itr = bufferList->begin();
						while (itr != bufferList->end())
						{
							Buffer* buffer = *itr;

							DebugLog("buffer info - buffer address : %p, %d, %d", buffer, buffer->startIndex, buffer->dataSize);

							if (buffer->dataSize == finishDataSize)
							{
								DebugLog("finish to process buffer");

								bufferList->erase(itr++);

								bufferPool->returnObject(buffer);

								break;
							}
							else if (buffer->dataSize < finishDataSize)
							{
								DebugLog("finish to process buffer - processing data size : %d, buffer data size : %d", usingDataSize, buffer->dataSize);

								finishDataSize -= buffer->dataSize;

								bufferList->erase(itr++);

								bufferPool->returnObject(buffer);

								continue;
							}
							else //������ �����Ͱ� ©��ä�� �� ���
							{
								DebugLog("not finish to process buffer");

								//���۸� ������ �ʰ� ������ ������ġ�� �����ͻ���� ��������
								buffer->startIndex += finishDataSize;
								buffer->dataSize -= finishDataSize;

								break;
							}
						}
					}
				}
				else if (dp->receiveType == RECEIVE_TYPE_CONNECT)
				{
					connector->onConnect(connectorInfo->hostId);
				}
				else if (dp->receiveType == RECEIVE_TYPE_DISCONNECT)
				{
					connector->onDisconnect(connectorInfo->hostId);

					//disconnect with connector and connector info
					if (connector->getConnectorType() == CONNECTOR_TYPE_SERVER)
					{
						if (((BaseServer*)connector)->connectorInfoMap.erase(connectorInfo->hostId) <= 0)
							ErrorLog("already remove connectorInfo");
					}
					else
					{
						BaseClient* client = ((BaseClient*)connectorInfo->connector);
						client->connectorInfo = nullptr;
					}

					connectorInfo->reset();

					//retun connectorInfo in pool
					Network::GetInstance()->connectorInfoPool->returnObject(connectorInfo);
				}
				else
				{
					ErrorLog("invalid receive type - %d", dp->receiveType);
				}

				dataPacketPool->returnObject(dp);
			}
			else
			{
				if (Network::GetInstance()->workerThreadCount != 0)
				{
					//never come here because multi thread wait when queue is empty
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