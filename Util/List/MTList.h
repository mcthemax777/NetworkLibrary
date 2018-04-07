#pragma once

#include <mutex>
#include <vector>
#include "List.h"
#include "Util/Lock/Lock.h"
namespace Util
{
	template < typename T >
	class MTList : public List<T> , public Lock
	{
	public:
		MTList()
		{
		}

		~MTList()
		{
		}

		T at(int index)
		{
			lock();

			if (index < 0 || index >= objectList.size())
			{
				unLock();

				return nullptr;
			}

			T t = objectList.at(index);

			unLock();

			return t;
		}

		bool remove(T t)
		{
			lock();

			for (int i = 0; i < objectList.size(); i++)
			{
				if (t == objectList.at(i))
				{
					objectList.erase(objectList.begin() + i);

					unLock();

					return true;
				}
			}

			unLock();

			return false;
		}

		bool remove(int index)
		{
			lock();

			if (index < 0 || index >= objectList.size())
			{
				unLock();

				return false;
			}

			objectList.erase(objectList.begin() + index);

			unLock();

			return true;
		}

		void push_back(T t)
		{
			lock();
			objectList.push_back(t);
			unLock();
		}

		int size()
		{
			lock();

			int size = objectList.size();

			unLock();

			return size;
		}

	protected:
		std::vector<T> objectList;
	};
}