#pragma once

#include <mutex>
#include <queue>
#include "Queue.h"

namespace Util
{
	template < typename T >
	class BQueue : public Queue<T>
	{
	public:
		BQueue()
		{
		}

		~BQueue()
		{
		}

		T pop()
		{
			T t = 0;

			{
				std::unique_lock<std::mutex> lock(mutex);

				if (objectQueue.empty())
				{
					cv.wait(lock);
				}

				t = objectQueue.front();
				objectQueue.pop();
			}

			return t;
		}

		void push(T t)
		{
			{
				std::lock_guard<std::mutex> lock(mutex);

				objectQueue.push(t);

				int size = objectQueue.size();
			}

			if (objectQueue.size() == 1)
			{
				cv.notify_all();
			}
		}

		int size()
		{
			int size;
			{
				std::lock_guard<std::mutex> lock(mutex);

				size = objectQueue.size();
			}

			return size;
		}

		std::queue<T> objectQueue;

		std::mutex mutex;
		std::condition_variable cv;
	};
}



