#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>

#include "SystemDefine.h"

typedef int logtype;

#if OS_PLATFORM == PLATFORM_WINDOWS
#else
typedef long time_t;
#endif


enum LogMode {
	LOG_MODE_SERVICE = 0,
	LOG_MODE_DEBUG,
};

enum LogType {
	LOG_TYPE_ERROR = 0,
	LOG_TYPE_DEBUG,
	LOG_TYPE_TRACE,
};

#define ErrorLog(fmt, ...)  Log::print(LOG_TYPE_ERROR, "%s,%s,%d,ERROR," fmt, __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__)
#define DebugLog(fmt, ...)  Log::print(LOG_TYPE_DEBUG, "%s,%s,%d,DEBUG," fmt, __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__)
#define TraceLog(fmt, ...)  Log::print(LOG_TYPE_TRACE, "%s,%s,%d,TRACE," fmt, __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__)


class Log
{
public:
	Log();
	~Log();

	bool initialize(const char* logpath);

	static bool createfile();

	static void print(logtype type, const char* fmt, ...);

private:
	static FILE* _fp;

	static time_t  _lastupdatetime;
};

#endif  // __LOG_H__