#pragma once

#include <stack>
#include "Stack.h"
#include "Util/Lock/SpinLock.h"

namespace Util
{
	template < typename T >
	class MTStack : public Stack<T>
	{
	public:
		MTStack()
		{
		}

		~MTStack()
		{
		}

		T get()
		{
			T t = 0;

			lock.lock();

			if (objectQueue.empty() == false)
			{
				t = objectStack.top();
			}

			lock.unLock();

			return t;
		}

		T pop()
		{
			T t = 0;

			lock.lock();

			if (objectStack.empty() == false)
			{
				t = objectStack.top();
				objectStack.pop();
			}

			lock.unLock();

			return t;
		}

		void push(T t)
		{
			lock.lock();

			objectStack.push(t);

			lock.unLock();
		}

		int size()
		{
			lock.lock();

			int size = objectStack.size();

			lock.unLock();

			return size;
		}

	protected:
		std::stack<T> objectStack;

		SpinLock lock;
	};
}