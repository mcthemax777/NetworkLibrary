#include "WorkerThread.h"
#include "Network.h"
#include "BaseConnector.h"

#include "Log/Log.h"
#include <cstdlib>

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
		while (!isStop)
		{
			DataPacket* dp = dataPacketQueue->pop();
			if (dp != NULL)
			{
				ConnectorInfo* connectorInfo = dp->connectorInfo;
				BaseConnector* connector = connectorInfo->connector;
				std::list<Buffer*>* bufferList = &(connectorInfo->bufferList);
				
				if (dp->receiveType == RECEIVE_TYPE_DATA)
				{
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

							if (buffer->dataSize = finishDataSize)
							{
								bufferList->erase(itr++);

								bufferPool->returnObject(buffer);

								break;
							}
							else if (buffer->dataSize < finishDataSize)
							{
								bufferList->erase(itr++);

								bufferPool->returnObject(buffer);

								finishDataSize -= buffer->dataSize;

								continue;
							}
							else //������ �����Ͱ� ©��ä�� �� ���
							{
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