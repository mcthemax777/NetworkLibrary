#pragma once

#include "SystemDefine.h"
#include "util/singleton/Singleton.h"
#include "util/lock/Lock.h"

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
	LOG_TYPE_DEBUG = 0,
	LOG_TYPE_INFO,
	LOG_TYPE_WARNING,
	LOG_TYPE_ERROR,
	LOG_TYPE_CRITICAL,
};

#define DebugLog(fmt, ...)  Log::GetInstance()->print(LOG_TYPE_DEBUG, "%s,%s,%d,DEBUG," fmt, __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__)
#define InfoLog(fmt, ...)  Log::GetInstance()->print(LOG_TYPE_INFO, "%s,%s,%d,INFO," fmt, __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__)
#define WarningLog(fmt, ...)  Log::GetInstance()->print(LOG_TYPE_WARNING, "%s,%s,%d,WARNING," fmt, __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__)
#define ErrorLog(fmt, ...)  Log::GetInstance()->print(LOG_TYPE_ERROR, "%s,%s,%d,ERROR," fmt, __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__)
#define CriticalLog(fmt, ...)  Log::GetInstance()->print(LOG_TYPE_CRITICAL, "%s,%s,%d,CRITICAL," fmt, __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__)

class Log : public Util::Singleton<Log>, public Util::Lock
{
public:
	Log();
	~Log();

	bool initialize(const char* logpath);

	bool createFile();

	void print(logtype type, const char* fmt, ...);

private:
	FILE* fp;
	char* logPath;
	time_t lastupdatetime;

	char* writeBuffer;
	char* buffer;

protected:
};