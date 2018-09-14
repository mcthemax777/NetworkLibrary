#pragma once

#include <list>
#include <thread>
#include <iostream>

namespace Util
{
	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief control thread
	*/
	class Thread
	{
	public:

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief create thread and run
		*/
		void start()
		{
			isStop = false;
			thrd = std::thread(&Thread::run, this);
			thrd.detach();
		}

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief change flag to stop thread
		*/
		void stop()
		{
			isStop = true;
		}

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief run thread
		*/
		virtual void run() = 0;

	protected:
		std::thread thrd;
		
		/// start and stop flag
		bool isStop;
	};
}