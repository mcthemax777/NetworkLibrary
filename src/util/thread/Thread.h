#pragma once

#include <list>
#include <thread>
#include <iostream>

namespace Util
{
	class Thread
	{
	public:

		void start()
		{
			isStop = false;
			thrd = std::thread(&Thread::run, this);
			thrd.detach();
		}

		void stop()
		{
			isStop = true;
		}

		virtual void run() = 0;

	protected:
		std::thread thrd;
		bool isStop;
	};
}