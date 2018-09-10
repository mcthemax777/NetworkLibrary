#pragma once

#include <mutex>
#include <vector>
#include "List.h"
#include "Util/Lock/Lock.h"
namespace Util
{
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

		T at(int index)
		{
			lock.lock();

			if (index < 0 || index >= objectList.size())
			{
				lock.unLock();

				return nullptr;
			}

			T t = objectList.at(index);

			lock.unLock();

			return t;
		}

		bool remove(T t)
		{
			lock.lock();

			for (int i = 0; i < objectList.size(); i++)
			{
				if (t == objectList.at(i))
				{
					objectList.erase(objectList.begin() + i);

					lock.unLock();

					return true;
				}
			}

			lock.unLock();

			return false;
		}

		bool remove(int index)
		{
			lock.lock();

			if (index < 0 || index >= objectList.size())
			{
				lock.unLock();

				return false;
			}

			objectList.erase(objectList.begin() + index);

			lock.unLock();

			return true;
		}

		void push_back(T t)
		{
			lock.lock();
			objectList.push_back(t);
			lock.unLock();
		}

		int size()
		{
			lock.lock();

			int size = objectList.size();

			lock.unLock();

			return size;
		}

	protected:
		std::vector<T> objectList;

		Lock lock;
	};
}