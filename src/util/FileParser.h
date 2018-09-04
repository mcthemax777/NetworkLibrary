#pragma once

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"

namespace Util
{
	class FileParser
	{
	public:
		static rapidjson::Document parseJson(const char* file);

	private:
		static FILE* getFile(const char* file);
	};

	rapidjson::Document FileParser::parseJson(const char* file)
	{
		FILE* fp = nullptr;

		if ((fp = getFile(file)) == nullptr)
		{
			printf("json file open error");
			getchar();
			exit(1);
		}

		char strTemp[5000];
		char *pStr = strTemp;

		memset(strTemp, 0, 5000);

		while (!feof(fp))
		{
			fgets(pStr, 5000, fp);
			pStr += strlen(pStr) - 1;
		}

		printf("%s\n%d", strTemp, strlen(strTemp));

		fclose(fp);

		rapidjson::Document document;
		document.Parse(strTemp);

		return document;
	}

	FILE* FileParser::getFile(const char* file)
	{
		FILE* fp = nullptr;

		if ((fp = fopen(file, "r")) == nullptr)
		{
			printf("json file open error");
		}

		return fp;
	}
}



