#pragma once

#include <functional>

namespace CG
{
	class Timer
	{
	protected:
		Timer(int _timerNo, int _interval, int _finishCount);

		//static int gettimeofday(struct timeval *tv, struct timezone *tz);
	public:
		std::function<void(time_t)> processTimerStart;
		std::function<void(time_t)> processTimerInterval;
		std::function<void(time_t)> processTimerFinish;

		friend class Network;

	protected:
		int timerNo;
		long startTime;
		int interval;
		int finishCount;


	};
}