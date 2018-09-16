#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <istream>
#include <unordered_map>

namespace Util
{
	/**
	* @author kim yong-chan
	* @date 2018-09-08
	* @brief convert file to use in cpp source
	*/
	class FileParser
	{
	public:
		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief get file and convert to key/value
		* @param const char* file : file path
		* @return std::unordered_map<std::string, std::string> : return key/value
		*/
		std::unordered_map<std::string, std::string> parseSettingFile(const char* file);

	private:
		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief check it is annotation line
		* @param std::string& line : one line in file
		* @return bool : if annotation line, return ture. or not, return false
		*/
		bool isAnnotationLine(std::string& line);

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief remove all empty character(" ")
		* @param std::string& line : one line in file
		*/
		void removeEmptySpace(std::string& line);

		/**
		* @author kim yong-chan
		* @date 2018-09-08
		* @brief convert line to key/value
		* @param std::string& line : one line in file
		* @return std::pair<std::string, std::string> : key/value pair
		* @todo if strange line??? think more
		*/
		std::pair<std::string, std::string> getKeyValue(std::string& line);

	};
}



