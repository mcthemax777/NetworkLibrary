#pragma once

#include <mutex>

namespace Util
{
	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief singleton
	*/
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
		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief get static instance
		* @return T* t : static instance
		*/
		static T * GetInstance()
		{
			//call once(create just one)
			std::call_once(m_flag,
				[]() { m_pInstance = new T; });
			if (m_pInstance == NULL)
				m_pInstance = new T;
			return m_pInstance;
		};

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief destroy instance
		*/
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
		static std::once_flag m_flag;
	};

	template <typename T> T * Singleton<T>::m_pInstance = 0;
	template <typename T> std::once_flag Singleton<T>::m_flag;
}



