#pragma once

#include <mutex>
#include "Util/Lock/SpinLock.h"

namespace Util
{
	class CondLock : public SpinLock
	{
	public:
		CondLock()
		{
			pthread_cond_init(&cond, NULL);
		}

		~CondLock()
		{
			pthread_cond_destroy(&cond);

		}

		void wait() { pthread_cond_wait(&cond, &mutex); }
		void signal() { pthread_cond_signal(&cond); }

		pthread_cond_t cond;
	};
}