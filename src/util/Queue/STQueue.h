#pragma once

#include <mutex>
#include <queue>
#include "Queue.h"
namespace Util
{
	template < typename T >
	class STQueue : public Queue<T>
	{
	public:
		STQueue()
		{
		}

		~STQueue()
		{
		}

		T pop()
		{
			T t;

			if (objectQueue.empty() == false)
			{
				t = objectQueue.front();
				objectQueue.pop();
				return t;
			}
			else
			{
				return 0;
			}
		}

		void push(T t) { objectQueue.push(t); }

		int size() { return objectQueue.size(); }

	protected:
		std::queue<T> objectQueue;
	};
}



