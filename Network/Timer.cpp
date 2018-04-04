#include "Timer.h"
#include "SystemDefine.h"


namespace CG
{
	Timer::Timer(int _timerNo, int _interval, int _finishCount)
	{
		timerNo = _timerNo;
		interval = _interval;
		finishCount = _finishCount;
		startTime = 0;
	}
}