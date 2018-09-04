#pragma once

#include <mutex>
#include <queue>
#include "Queue.h"
#include "Util/Lock/CondLock.h"

namespace Util
{
	template < typename T >
	class BQueue : public Queue<T>, public CondLock
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

			lock();

			if (objectQueue.empty())
			{
				wait();
			}

			t = objectQueue.front();
			objectQueue.pop();

			unLock();

			return t;
		}

		void push(T t)
		{
			lock();

			objectQueue.push(t);

			if (objectQueue.size() == 1)
				signal();

			unLock();
		}

		int size()
		{
			lock();

			int size = objectQueue.size();

			unLock();

			return size;
		}

		std::queue<T> objectQueue;
	};
}



