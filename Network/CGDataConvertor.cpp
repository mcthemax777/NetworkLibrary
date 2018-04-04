#include "CGDataConvertor.h"

namespace CG
{
	int CGDataConvertor::convert(char* data, int dataSize)
	{
		return 1;
//		char* wholeRecvBuffer = data;
//		int wholeRecvBufferSize = dataSize;
//		int recvCnt = dataSize;
//		char* allocateRecvBuffer = NULL;
//
//
//		CharString* tempBufferInfo = &connectInfo->tempBufferInfo;
//
//		int tempBufferSize = tempBufferInfo->stringSize;
//
//
//		if (tempBufferSize != 0)
//		{
//			wholeRecvBuffer = (char*)malloc(sizeof(char) * (recvCnt + tempBufferSize));
//
//			memcpy(wholeRecvBuffer, tempBufferInfo->string, tempBufferSize);
//			memcpy(wholeRecvBuffer + tempBufferSize, recvBuffer, recvCnt);
//			wholeRecvBufferSize += tempBufferInfo->stringSize;
//
//			connectInfo->tempBufferInfo.stringSize = 0;
//			free(connectInfo->tempBufferInfo.string);
//			connectInfo->tempBufferInfo.string = NULL;
//
//			allocateRecvBuffer = wholeRecvBuffer;
//		}
//
//		char* pWholeRecvBuffer = wholeRecvBuffer;
//		int pWholeRecvBufferSize = wholeRecvBufferSize;
//
//		while (true)
//		{
//			if (pWholeRecvBufferSize < sizeof(dataType_t))
//			{
//				connectInfo->tempBufferInfo.stringSize = wholeRecvBufferSize;
//				connectInfo->tempBufferInfo.string = (char*)malloc(sizeof(char) * wholeRecvBufferSize);
//				memcpy(connectInfo->tempBufferInfo.string, wholeRecvBuffer, wholeRecvBufferSize);
//
//				break;
//			}
//
//			////////////////////////// data type
//			dataType_t flags;
//			memcpy(&flags, pWholeRecvBuffer, sizeof(flags));
//			pWholeRecvBuffer += sizeof(flags);
//			pWholeRecvBufferSize -= sizeof(flags);
//
//			if (flags == DATA_TYPE_PING_NOTIFY)
//			{
//				dataType_t dataTypeToSend = DATA_TYPE_PING_OK;
//				sendData(connectInfo, (char*)&dataTypeToSend, sizeof(dataTypeToSend));
//
//				if (pWholeRecvBufferSize == 0)
//				{
//					break;
//				}
//				else if (pWholeRecvBufferSize > 0) // 남은 데이터들 존재함
//				{
//					wholeRecvBufferSize = pWholeRecvBufferSize;
//					wholeRecvBuffer = pWholeRecvBuffer;
//
//					continue;
//				}
//				else
//				{
//					ErrorLog("pWholeRecvBufferSize - %d", pWholeRecvBufferSize);
//					return;
//				}
//
//			}
//			else if (flags == DATA_TYPE_PING_OK)
//			{
//				connectInfo->sendPingCount = 0;
//
//				if (pWholeRecvBufferSize == 0)
//				{
//					break;
//				}
//				else if (pWholeRecvBufferSize > 0) // 남은 데이터들 존재함
//				{
//					wholeRecvBufferSize = pWholeRecvBufferSize;
//					wholeRecvBuffer = pWholeRecvBuffer;
//
//					continue;
//				}
//				else
//				{
//					ErrorLog("pWholeRecvBufferSize - %d", pWholeRecvBufferSize);
//					//return ;
//				}
//			}
//			else if (flags == DATA_TYPE_REQ)
//			{
//				if (pWholeRecvBufferSize < sizeof(dataSize_t))
//				{
//					connectInfo->tempBufferInfo.stringSize = wholeRecvBufferSize;
//					connectInfo->tempBufferInfo.string = (char*)malloc(wholeRecvBufferSize);
//					memcpy(connectInfo->tempBufferInfo.string, wholeRecvBuffer, wholeRecvBufferSize);
//
//					break;
//				}
//
//				dataSize_t dataSize;
//				memcpy(&dataSize, pWholeRecvBuffer, sizeof(dataSize));
//				pWholeRecvBuffer += sizeof(dataSize);
//				pWholeRecvBufferSize -= sizeof(dataSize);
//
//
//				if (dataSize <= pWholeRecvBufferSize)
//				{
//#if THREAD_TYPE == SINGLE_THREAD
//					workerThreadArray[0]->receiveData(connectInfo, pWholeRecvBuffer, (int)dataSize);
//#else
//					if ((connectInfo->flags & FLAG_PROCESSING) != 0)
//					{
//						CharString* saveData = new CharString();
//						saveData->deepCopy((int)dataSize, pWholeRecvBuffer);
//						connectInfo->tempDataQueue.push_back(saveData);
//					}
//					else
//					{
//						connectInfo->flags |= FLAG_PROCESSING;
//						sendDataToWorkerThread(RECEIVE_TYPE_RECEIVE, connectInfo, pWholeRecvBuffer, (int)dataSize);
//					}
//#endif
//
//					pWholeRecvBuffer += dataSize;
//					pWholeRecvBufferSize -= dataSize;
//
//					if (pWholeRecvBufferSize == 0)
//					{
//						break;
//					}
//					else if (pWholeRecvBufferSize > 0)
//					{
//						wholeRecvBufferSize = pWholeRecvBufferSize;
//						wholeRecvBuffer = pWholeRecvBuffer;
//
//						continue;
//					}
//					else
//					{
//						ErrorLog("pWholeRecvBufferSize - %d", pWholeRecvBufferSize);
//					}
//				}
//				else
//				{
//					connectInfo->tempBufferInfo.deepCopy(wholeRecvBufferSize, wholeRecvBuffer);
//					break;
//				}
//			}
//
//		}
//
//		if (allocateRecvBuffer != NULL)
//		{
//			free(allocateRecvBuffer);
//		}
	}
}
