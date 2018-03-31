#define _CRT_SECURE_NO_WARNINGS
#define HAVE_STRUCT_TIMESPEC


#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <iostream>
#include <pthread.h>

#include "Log.h"


Log::Log()
{
	fp = nullptr;
	lastupdatetime = 0;

	logPath = new char[128];
	memset(logPath, 0, 128);
	memcpy(logPath, "../../Log/GameLog/GameLog_", 25);

	writeBuffer = new char[5000];
	buffer = new char[5000];

	pthread_mutex_init(&mutex, NULL);
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

	pthread_mutex_destroy(&mutex);
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
		exit(1);
	}

	return true;
}

void Log::print(logtype type, const char* fmt, ...)
{
	struct tm* prevtimeinfo = localtime(&lastupdatetime);
	int createdday = prevtimeinfo->tm_mday;
	int createdhour = prevtimeinfo->tm_hour;

	time_t curtime = time(nullptr);
	struct tm* timeinfo = localtime(&curtime);

	lock();

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

	sprintf(buffer, "[%d-%02d-%02d %02d:%02d:%02d][thread %p] %s", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, std::this_thread::get_id, writeBuffer);

	puts(buffer);

	if (fp)
	{
		fprintf(fp, "%s\n", buffer);
		fflush(fp);
	}

	unLock();
}