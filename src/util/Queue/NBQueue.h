#pragma once

#include <queue>
#include "Queue.h"
#include "Util/Lock/SpinLock.h"

namespace Util
{
	template < typename T >
	class NBQueue : public Queue<T>, public SpinLock
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

			lock();

			if (objectQueue.empty() == false)
			{
				t = objectQueue.front();
				objectQueue.pop();
			}

			unLock();

			return t;
		}

		void push(T t)
		{
			lock();

			objectQueue.push(t);

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