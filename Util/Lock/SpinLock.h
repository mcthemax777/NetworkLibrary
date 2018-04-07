#pragma once

#include <mutex>

namespace Util
{
	class SpinLock
	{
	public:
		SpinLock()
		{
			pthread_mutex_init(&mutex, NULL);
		}

		~SpinLock()
		{
			pthread_mutex_destroy(&mutex);
		}

		void lock() { while (pthread_mutex_trylock(&mutex) != 0); }
		void unLock() { pthread_mutex_unlock(&mutex); }

	protected:
		pthread_mutex_t mutex;
	};
}