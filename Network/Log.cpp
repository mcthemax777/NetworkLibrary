#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include <stdarg.h>
#include <time.h>


#include "Log.h"

FILE* Log::_fp = nullptr;
time_t Log::_lastupdatetime = 0;

static const int file_name_len = 128;
static char _logpath[file_name_len];


Log::Log()
{
	_fp = nullptr;
	_lastupdatetime = 0;
}

Log::~Log()
{
	if (_fp)
	{
		fclose(_fp);
	}
}

bool Log::initialize(const char* logpath)
{
	memset(_logpath, 0, sizeof(_logpath));
	memcpy(_logpath, logpath, strlen(logpath));

	return createfile();
}

bool Log::createfile()
{
	if (_fp)
	{
		fclose(_fp);
	}

	time_t curtime;
	struct tm* timeinfo;

	curtime = time(nullptr);
	timeinfo = localtime(&curtime);

	_lastupdatetime = curtime;

	char fullfilename[file_name_len] = { 0, };

	sprintf(fullfilename, "%s%d%02d%02d_%02d%02d%02d%s",
		_logpath, timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, ".txt");

	if ((_fp = fopen(fullfilename, "w")) == nullptr)
	{
		ErrorLog("log file open error");
		return false;
	}

	return true;
}

void Log::print(logtype type, const char* fmt, ...)
{
	struct tm* prevtimeinfo = localtime(&_lastupdatetime);
	int createdday = prevtimeinfo->tm_mday;
	int createdhour = prevtimeinfo->tm_hour;

	time_t curtime;
	struct tm* timeinfo;

	curtime = time(nullptr);
	timeinfo = localtime(&curtime);

	if (createdday != timeinfo->tm_mday ||
		createdhour != timeinfo->tm_hour)
	{
		if (!createfile())
			return;
	}

	static char logbuf[7000];
	static char buf[7000];

	memset(logbuf, 0, sizeof(logbuf));
	memset(buf, 0, sizeof(buf));

	va_list list;

	va_start(list, fmt);
	vsprintf(logbuf, fmt, list);
	va_end(list);

	sprintf(buf, "%d-%02d-%02d %02d:%02d:%02d,%s", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, logbuf);

	puts(buf);

	if (_fp)
	{
		fprintf(_fp, "%s\n", buf);
		fflush(_fp);
	}
}