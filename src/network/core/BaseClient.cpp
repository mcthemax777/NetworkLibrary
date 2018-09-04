#include "BaseClient.h"
#include "Network.h"
#include "Log/Log.h"
#include "WorkerThread.h"

namespace CG
{

	void BaseClient::processMessage()
	{
		if (Network::GetInstance()->workerThreadCount == 0)
		{
			(*(Network::GetInstance()->workerThreadArray))->run();
		}
	}
}
