#pragma once

#include "Log/Log.h"
#include "Util/Queue/STQueue.h"
#include "Util/Queue/NBQueue.h"


namespace Util
{
	template < typename T >
	class ObjectPool
	{
	public:
		ObjectPool(unsigned int capacity, bool isUsingMultiThread)
		{
			this->capacity = capacity;
			this->isUsingMultiThread = isUsingMultiThread;

			if (isUsingMultiThread) objectList = new NBQueue<T*>();
			else objectList = new STQueue<T*>();

			for (int i = 0; i < capacity; i++)
				objectList->push(new T());

		}

		virtual ~ObjectPool()
		{
			delete objectList;
		}

		T* getObject()
		{
			T* t = 0;

			t = objectList->pop();

			if (t == 0)
			{
				DebugLog("already use all object in objectPool");
				t = new T();
			}

			return t;
		}

		void returnObject(T* object)
		{
			if (objectList->size() >= capacity)
			{
				delete object;
			}
			else
			{
				objectList->push(object);
			}
		}

		int size()
		{
			int size = objectList->size();

			return size;
		}

	protected:
		Util::Queue<T*>* objectList;

		unsigned int capacity;
		bool isUsingMultiThread;
	};
}



