#pragma once
namespace Util
{
	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief custom list interface
	*/
	template < typename T >
	class List
	{
	public:
		virtual ~List() {}
		
		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief get T by index
		* @param int index : index to find
		* @return T result : finding T
		*/
		virtual T at(int index) = 0;
		
		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief remove T
		* @param T t : will remove item
		* @return bool result : is success remove
		*/
		virtual bool remove(T t) = 0;
		
		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief remove T
		* @param int index : removing T's index
		* @return bool result : is success remove
		*/
		virtual bool remove(int index) = 0;

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief insert back
		* @param T t : inserting item
		*/
		virtual void push_back(T t) = 0;

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief get list size
		* @return int result : list size
		*/
		virtual int size() = 0;
	};
}



