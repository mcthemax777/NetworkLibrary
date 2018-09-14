#pragma once

#include "Log/Log.h"
#include "Util/Queue/STQueue.h"
#include "Util/Queue/NBQueue.h"


namespace Util
{
	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief pre allocate, so reduce cpu calculation
	*/
	template < typename T >
	class ObjectPool
	{
	public:
		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief init member value
		* @param unsigned int capacity : create object count
		* @param bool isUsingMultiThread : if multi-thread, using lock
		*/
		ObjectPool(unsigned int capacity, bool isUsingMultiThread)
		{
			//set member value
			this->capacity = capacity;
			this->isUsingMultiThread = isUsingMultiThread;

			//set queue
			if (isUsingMultiThread) objectList = new NBQueue<T*>();
			else objectList = new STQueue<T*>();

			//create object as much as capacity
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



