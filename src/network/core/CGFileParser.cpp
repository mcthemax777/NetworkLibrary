#include "CGFileParser.h"

namespace CG
{
	std::unordered_map<std::string, std::string> CGFileParser::parseSettingFile(const char* file)
	{
		std::unordered_map<std::string, std::string> settingMap;

		//get file in stream
		std::ifstream infile(file);

		std::string line;

		//read line repeatily and save line if it is not annotation
		while (std::getline(infile, line))
		{
			//if it is annotation, read next line
			if (isAnnotationLine(line))
			{
				continue;
			}

			//remove all empty space
			removeEmptySpace(line);

			//save key, value in map;
			std::pair<std::string, std::string> kv = getKeyValue(line);
			settingMap.insert(kv);

		}

		return settingMap;
	}

	bool CGFileParser::isAnnotationLine(std::string& line)
	{
		if (line.at(0) == '#')
			return true;

		return false;
	}

	void CGFileParser::removeEmptySpace(std::string& line)
	{
		//change " " -> "" in line
		size_t pos;
		while ((pos = line.find(" ")) != std::string::npos)
		{
			line.replace(pos, 1, "");
		}
	}

	std::pair<std::string, std::string> CGFileParser::getKeyValue(std::string& line)
	{
		//split 2 line by '='
		size_t pos;
		if ((pos = line.find("=")) == std::string::npos)
		{
			return std::pair<std::string, std::string>("", "");
		}
		else
		{
			//return key/value fommatting pair
			return std::pair<std::string, std::string>(line.substr(0, pos), line.substr(pos + 1));
		}
	}
}



