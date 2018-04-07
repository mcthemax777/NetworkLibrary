#pragma once

#include <mutex>
#include <vector>
#include "List.h"
namespace Util
{
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

		T at(int index)
		{
			if (index < 0 || index >= size())
			{
				return nullptr;
			}

			return objectList.at(index);
		}

		bool remove(T t)
		{
			for (int i = 0; i < size(); i++)
			{
				if (t == objectList.at(i))
				{
					objectList.erase(t);
					return true;
				}
			}

			return false;
		}

		bool remove(int index)
		{
			if (index < 0 || index >= size())
			{
				return false;
			}

			objectList.erase(objectList.at(index));
			return true;
		}

		void push_back(T t)
		{
			objectList.push_back(t);
		}

		int size()
		{
			return objectList.size();
		}

	protected:
		std::vector<T> objectList;
	};
}