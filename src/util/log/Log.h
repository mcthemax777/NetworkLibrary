#pragma once

#include "SystemDefine.h"
#include "util/singleton/Singleton.h"
#include "util/lock/Lock.h"

typedef int logtype;

#if OS_PLATFORM == PLATFORM_WINDOWS
#else
typedef long time_t;
#endif

#define DebugLog(fmt, ...)  Util::Log::GetInstance()->print(Util::LOG_TYPE_DEBUG, "%s,%s,%d,DEBUG," fmt, __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__)
#define InfoLog(fmt, ...)  Util::Log::GetInstance()->print(Util::LOG_TYPE_INFO, "%s,%s,%d,INFO," fmt, __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__)
#define WarningLog(fmt, ...)  Util::Log::GetInstance()->print(Util::LOG_TYPE_WARNING, "%s,%s,%d,WARNING," fmt, __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__)
#define ErrorLog(fmt, ...)  Util::Log::GetInstance()->print(Util::LOG_TYPE_ERROR, "%s,%s,%d,ERROR," fmt, __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__)
#define CriticalLog(fmt, ...)  Util::Log::GetInstance()->print(Util::LOG_TYPE_CRITICAL, "%s,%s,%d,CRITICAL," fmt, __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__)


namespace Util
{
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

	class Log : public Util::Singleton<Log>
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
		logtype logLevel;
		time_t lastupdatetime;

		char* writeBuffer;
		char* buffer;

		Util::Lock lock;
	};
}