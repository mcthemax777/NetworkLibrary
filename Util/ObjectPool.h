#pragma once

#include <mutex>
#include <stack>

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

			for (int i = 0; i < capacity; i++)
			{
				T* t = new T();
				objectList.push(t);
			}

			pthread_mutex_init(&mutex, NULL);
		}
		virtual ~ObjectPool()
		{

		}

		T* getObject()
		{
			T* t;

			lock();

			if (objectList.empty())
			{
				t = new T();
			}
			else
			{
				t = objectList.top();
				objectList.pop();
			}

			unLock();

			return t;

		}

		void returnObject(T* object)
		{
			lock();

			if (objectList.size() >= capacity)
			{
				unLock();
				delete object;
			}
			else
			{
				objectList.push(object);
				unLock();
			}
		}

		int objectCount()
		{
			lock();

			int size = objectList.size();

			unLock();

			return size;
		}
		
		void lock() 
		{
			if(isUsingMultiThread)
				pthread_mutex_lock(&mutex);
		}
		void unLock() 
		{ 
			if (isUsingMultiThread)
				pthread_mutex_unlock(&mutex); 
		}

	private:

	protected:
		pthread_mutex_t mutex;

		unsigned int capacity;
		bool isUsingMultiThread;
		std::stack<T*> objectList;
	};
}



