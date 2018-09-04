#pragma once
namespace Util
{
	template < typename T >
	class List
	{
	public:
		virtual ~List() {}
		virtual T at(int index) = 0;
		virtual bool remove(T t) = 0;
		virtual bool remove(int index) = 0;
		virtual void push_back(T t) = 0;
		virtual int size() = 0;
	};
}



