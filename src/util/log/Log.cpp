#define _CRT_SECURE_NO_WARNINGS


#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <iostream>
#include <thread>
#include <sstream>

#include "Log.h"
#include "../parser/FileParser.h"

namespace Util
{
	Log::Log()
	{
		fp = nullptr;
		lastupdatetime = 0;

		logPath = new char[128];
		memset(logPath, 0, 128);

		//get network setting from file
		FileParser fp;
		std::unordered_map<std::string, std::string> kv = fp.parseSettingFile("log_config.cg");

		//get worker thread count 
		std::string logPathStr = kv["log_path"];

		memcpy(logPath, logPathStr.c_str(), logPathStr.length());

		//get log level
		std::string logLevelStr = kv["log_level"];

		//set log level
		if (logLevelStr.compare("DEBUG") == 0)
		{
			logLevel = LOG_TYPE_DEBUG;
		}
		else if (logLevelStr.compare("INFO") == 0)
		{
			logLevel = LOG_TYPE_INFO;
		}
		else if (logLevelStr.compare("WARNING") == 0)
		{
			logLevel = LOG_TYPE_WARNING;
		}
		else if (logLevelStr.compare("ERROR") == 0)
		{
			logLevel = LOG_TYPE_ERROR;
		}
		else if (logLevelStr.compare("CRITICAL") == 0)
		{
			logLevel = LOG_TYPE_CRITICAL;
		}
		else
		{
			printf("log type error - %s", logLevelStr.c_str());
			logLevel = LOG_TYPE_CRITICAL;
		}

		writeBuffer = new char[5000];
		buffer = new char[5000];

	}

	Log::~Log()
	{
		if (fp)
		{
			fclose(fp);
		}

		delete logPath;
		delete buffer;
		delete writeBuffer;

	}

	bool Log::initialize(const char* _logpath)
	{
		return true;
	}

	bool Log::createFile()
	{


		if (fp)
		{
			fclose(fp);
		}

		time_t curtime;
		struct tm* timeinfo;

		curtime = time(nullptr);
		timeinfo = localtime(&curtime);

		lastupdatetime = curtime;

		char fullfilename[128] = { 0, };

		sprintf(fullfilename, "%s%d%02d%02d_%02d%02d%02d%s",
			logPath, timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, ".txt");

		if ((fp = fopen(fullfilename, "w")) == nullptr)
		{
			printf("log file open error");
			getchar();
			exit(1);
		}

		return true;
	}

	void Log::print(logtype type, const char* fmt, ...)
	{
		//log level smail than type
		if (logLevel > type)
		{
			return;
		}

		struct tm* prevtimeinfo = localtime(&lastupdatetime);
		int createdday = prevtimeinfo->tm_mday;
		int createdhour = prevtimeinfo->tm_hour;

		time_t curtime = time(nullptr);
		struct tm* timeinfo = localtime(&curtime);

		lock.lock();

		if (createdday != timeinfo->tm_mday ||
			createdhour != timeinfo->tm_hour)
		{
			if (!createFile())
				return;
		}

		memset(buffer, 0, 5000);
		memset(writeBuffer, 0, 5000);

		va_list list;

		va_start(list, fmt);
		vsprintf(writeBuffer, fmt, list);
		va_end(list);

		std::stringstream ss;
		ss << std::this_thread::get_id();
		uint64_t id = std::stoull(ss.str());

#if OS_PLATFORM == PLATFORM_WINDOWS
		sprintf(buffer, "[%d-%02d-%02d %02d:%02d:%02d][thread %lld]%s", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, id, writeBuffer);
#else
		sprintf(buffer, "[%d-%02d-%02d %02d:%02d:%02d][thread %lld]%s", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, id, writeBuffer);
#endif
		puts(buffer);

		if (fp)
		{
			fprintf(fp, "%s\n", buffer);
			fflush(fp);
		}

		lock.unLock();
	}
}