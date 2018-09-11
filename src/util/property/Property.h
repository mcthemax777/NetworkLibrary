#pragma once

namespace Util
{
	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief give get, set method easily
	*/
	template <typename T>
	class Property {
	public:
		virtual ~Property() {}

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief set function
		*/
		virtual T & operator = (const T &f) { return value = f; }
		
		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief get function
		*/
		virtual operator T const & () const { return value; }
	protected:

		///storage member value
		T value;
	};
}



