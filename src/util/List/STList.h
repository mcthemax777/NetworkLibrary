#pragma once

#include <mutex>
#include <vector>
#include "List.h"
namespace Util
{
	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief custom non-thread-safe list
	*/
	template < typename T >
	class STList : public List<T>
	{
	public:
		STList()
		{
		}

		~STList()
		{
		}

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief see class 'List'
		*/
		T at(int index)
		{
			//invalid index
			if (index < 0 || index >= size())
			{
				return nullptr;
			}

			return objectList.at(index);
		}

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief see class 'List'
		*/
		bool remove(T t)
		{
			//run loop to find item
			for (int i = 0; i < size(); i++)
			{
				//find item
				if (t == objectList.at(i))
				{
					//remove item
					objectList.erase(t);

					return true;
				}
			}

			return false;
		}

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief see class 'List'
		*/
		bool remove(int index)
		{
			//invalid index
			if (index < 0 || index >= size())
			{
				return false;
			}

			//remove item by index
			objectList.erase(objectList.at(index));
			
			return true;
		}

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief see class 'List'
		*/
		void push_back(T t)
		{
			//insert item back
			objectList.push_back(t);
		}

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief see class 'List'
		*/
		int size()
		{
			//return list size
			return objectList.size();
		}

	protected:
		///storage list
		std::vector<T> objectList;
	};
}