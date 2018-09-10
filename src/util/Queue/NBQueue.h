#pragma once

#include <queue>
#include "Queue.h"
#include "Util/Lock/SpinLock.h"

namespace Util
{
	template < typename T >
	class NBQueue : public Queue<T>
	{
	public:
		NBQueue()
		{
		}

		~NBQueue()
		{
		}

		T pop()
		{
			T t = 0;

			lock.lock();

			if (objectQueue.empty() == false)
			{
				t = objectQueue.front();
				objectQueue.pop();
			}

			lock.unLock();

			return t;
		}

		void push(T t)
		{
			lock.lock();

			objectQueue.push(t);

			lock.unLock();
		}

		int size()
		{
			lock.lock();

			int size = objectQueue.size();

			lock.unLock();

			return size;
		}

	protected:
		std::queue<T> objectQueue;

		SpinLock lock;
	};
}