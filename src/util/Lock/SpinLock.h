#pragma once

#include <mutex>

namespace Util
{
	class SpinLock
	{
	public:
		SpinLock()
		{
			
		}

		~SpinLock()
		{
			
		}

		void lock() { while (!mutex.try_lock()); }
		void unLock() { mutex.unlock(); }

	protected:
		std::mutex mutex;
	};
}