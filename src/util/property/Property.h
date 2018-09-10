#pragma once

namespace Util
{
	template <typename T>
	class Property {
	public:
		virtual ~Property() {}
		virtual T & operator = (const T &f) { return value = f; }
		virtual operator T const & () const { return value; }
	protected:
		T value;
	};
}



