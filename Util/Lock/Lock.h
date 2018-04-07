#pragma once

#include <mutex>

namespace Util
{
	class Lock
	{
	protected:
		Lock()
		{
			pthread_mutex_init(&mutex, NULL);
		}

		~Lock()
		{
			pthread_mutex_destroy(&mutex);
		}

		void lock() { pthread_mutex_lock(&mutex); }
		void unLock() { pthread_mutex_unlock(&mutex); }

	protected:
		pthread_mutex_t mutex;
	};
}



