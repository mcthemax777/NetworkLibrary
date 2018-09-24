#pragma once

#include <stack>
#include "Stack.h"
namespace Util
{
	template < typename T >
	class STStack : public Stack<T>
	{
	public:
		STStack()
		{
		}

		~STStack()
		{
		}

		T get()
		{
			T t;

			if (objectStack.empty() == false)
			{
				t = objectStack.top();
				return t;
			}
			else
			{
				return 0;
			}
		}

		T pop()
		{
			T t;

			if (objectStack.empty() == false)
			{
				t = objectStack.top();
				objectStack.pop();
				return t;
			}
			else
			{
				return 0;
			}
		}

		void push(T t) { objectStack.push(t); }

		int size() { return objectStack.size(); }

	protected:
		std::stack<T> objectStack;
	};
}



