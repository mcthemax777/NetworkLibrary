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

					//받은 버퍼를 아직 처리되지 않은 버퍼리스트에 추가
					connectorInfo->bufferList.push_back(dp->buffer);

					int dataSize = 0;

					//버퍼에 쌓인 데이터 크기의 배열 만들기
					for (Buffer* buffer : *bufferList)
					{
						dataSize += buffer->dataSize;
					}

					char* data = new char[dataSize];

					//데이터를 카피.
					int copySize = 0;
					for (Buffer* buffer : *bufferList)
					{
						memcpy(data + copySize, buffer->data + buffer->startIndex, buffer->dataSize);
						copySize += buffer->dataSize;
					}

					//받은 데이터를 사용자가 등록한대로 변환하여 전송
					int usingDataSize = connector->processData(connectorInfo, data, dataSize);

					DebugLog("processing data size - %d", usingDataSize);

					//동적할당한 data를 삭제
					delete data;

					if (usingDataSize == 0) //데이터가 덜 와서 그냥 아무것도 안함.
					{

					}
					else
					{
						//저장하고 있는 버퍼들중 사용이 끝난 버퍼는 풀에 다시 보냄
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
							else //마지막 데이터가 짤린채로 올 경우
							{
								DebugLog("not finish to process buffer");

								//버퍼를 지우지 않고 데이터 시작위치와 데이터사이즈만 조절해줌
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