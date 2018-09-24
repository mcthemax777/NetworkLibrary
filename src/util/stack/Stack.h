#pragma once
namespace Util
{
	template < typename T >
	class Stack
	{
	public:
		virtual ~Stack() {}
		virtual T pop() = 0;
		virtual void push(T t) = 0;
		virtual int size() = 0;
	};
}



