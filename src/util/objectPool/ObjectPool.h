#pragma once

#include "Log/Log.h"
#include "Util/Queue/STQueue.h"
#include "Util/Queue/NBQueue.h"


namespace Util
{
	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief pool to reduce allocating
	*/
	template < typename T >
	class ObjectPool
	{
	public:

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief init member value
		* @param unsigned int capacity : create item count when init
		* @param bool isUsingMultiThread : is multi-thread
		*/
		ObjectPool(unsigned int capacity, bool isUsingMultiThread)
		{
			this->capacity = capacity;
			this->isUsingMultiThread = isUsingMultiThread;

			//if multi-thread, thread-safe queue
			if (isUsingMultiThread) objectList = new NBQueue<T*>();
			else objectList = new STQueue<T*>();

			//create item as much as capacity
			for (int i = 0; i < capacity; i++)
				objectList->push(new T());

		}

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief delete list
		*/
		virtual ~ObjectPool()
		{
			delete objectList;
		}

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief return 1 item
		* @return T* result : return item
		*/
		T* getObject()
		{
			T* t = 0;

			//get item from queue
			t = objectList->pop();

			//not exist, create item
			if (t == 0)
			{
				DebugLog("already use all object in objectPool");
				t = new T();
			}

			//return item
			return t;
		}


		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief insert item
		* @param T* object : insert item
		*/
		void returnObject(T* object)
		{
			//if bigger than capacity, delete item
			if (objectList->size() >= capacity)
			{
				delete object;
			}
			else
			{
				//insert item
				objectList->push(object);
			}
		}

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief return pool size
		* @return int result : pool size
		*/
		int size()
		{
			//get pool size
			int size = objectList->size();

			return size;
		}

	protected:
		///storage items
		Util::Queue<T*>* objectList;

		///max pool size
		unsigned int capacity;
		
		///is multi-thread
		bool isUsingMultiThread;
	};
}



