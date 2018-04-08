#pragma once

#define HAVE_STRUCT_TIMESPEC

#include <mutex>
#include <pthread.h>

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



