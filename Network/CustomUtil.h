#pragma once
#include "SystemDefine.h"

namespace CustomUtil
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

	template < typename T >
	class Singleton
	{
	protected:
		Singleton()
		{

		}
		virtual ~Singleton()
		{

		}

	public:
		static T * GetInstance()
		{
			if (m_pInstance == NULL)
				m_pInstance = new T;
			return m_pInstance;
		};

		static void DestroyInstance()
		{
			if (m_pInstance)
			{
				delete m_pInstance;
				m_pInstance = NULL;
			}
		};

	private:
		static T * m_pInstance;
	};

	template <typename T> T * Singleton<T>::m_pInstance = 0;
}



