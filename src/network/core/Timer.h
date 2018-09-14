#pragma once

#include <functional>

namespace CG
{
	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief if you want to regist timer, using this.
	*/
	class Timer
	{
	public:
		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief set timer info
		* @param int _timerNo : timer number. when you want to find this, you can find by number
		* @param int _interval : set call interval
		* @param int _finishCount : set finish count
		*/
		Timer(int _timerNo, int _interval, int _finishCount);

		///when timer start, call this function
		std::function<void(time_t)> processTimerStart;

		///when timer call interval, call this function
		std::function<void(time_t)> processTimerInterval;

		///when timer finish, call this function
		std::function<void(time_t)> processTimerFinish;

	protected:
		///timer number
		int timerNo;
		
		///start time
		long startTime;
		
		///call interval
		int interval;
		
		///finish count (if 0, repeat forever)
		int finishCount;

	public:
		friend class Network;
	};
}