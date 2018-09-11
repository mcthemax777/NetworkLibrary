#pragma once

#include <mutex>
#include <vector>
#include "List.h"
#include "Util/Lock/Lock.h"
namespace Util
{
	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief custom thread-safe list
	*/
	template < typename T >
	class MTList : public List<T>
	{
	public:
		MTList()
		{
		}

		~MTList()
		{
		}

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief see class 'List'
		*/
		T at(int index)
		{
			lock.lock();

			//not exist item or invalid index
			if (index < 0 || index >= objectList.size())
			{
				lock.unLock();

				// return null
				return nullptr;
			}

			//get item by index
			T t = objectList.at(index);

			lock.unLock();

			//return item
			return t;
		}

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief see class 'List'
		*/
		bool remove(T t)
		{
			lock.lock();

			//run loop to find t
			for (int i = 0; i < objectList.size(); i++)
			{
				//if find item
				if (t == objectList.at(i))
				{
					//remove item
					objectList.erase(objectList.begin() + i);

					lock.unLock();

					return true;
				}
			}

			lock.unLock();

			return false;
		}

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief see class 'List'
		*/
		bool remove(int index)
		{
			lock.lock();

			//not exist item or invalid index
			if (index < 0 || index >= objectList.size())
			{
				lock.unLock();

				//return null
				return false;
			}

			//remove item
			objectList.erase(objectList.begin() + index);

			lock.unLock();

			return true;
		}

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief see class 'List'
		*/
		void push_back(T t)
		{
			lock.lock();

			//push item
			objectList.push_back(t);
			
			lock.unLock();
		}

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief see class 'List'
		*/
		int size()
		{
			lock.lock();

			//get list size
			int size = objectList.size();

			lock.unLock();

			return size;
		}

	protected:
		///storage item
		std::vector<T> objectList;

		///lock
		Lock lock;
	};
}