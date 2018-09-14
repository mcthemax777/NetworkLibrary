#pragma once

#define HAVE_STRUCT_TIMESPEC

#include <mutex>

namespace Util
{
	class Lock
	{
	public:

		~Lock()
		{
			
		}

		void lock() { mutex.lock(); }
		void unLock() { mutex.unlock(); }

	protected:
		std::mutex mutex;
	};
}



