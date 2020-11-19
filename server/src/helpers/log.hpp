//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-11-19                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#ifndef LOG_H
#define LOG_H
#include <string>

class Log
{
	public:
		Log();
		~Log();

		static void verbose(std::string tag, std::string text, bool showTag=true);
		static void debug(std::string tag, std::string text, bool showTag=true);
		//static void debug_pair(std::string tag, std::string label, std::string text);
		static void success(std::string tag, std::string text, bool showTag=true);
		static void info(std::string tag, std::string text, bool showTag=true);
		static void infoItem(std::string tag, std::string text);
		//static void info_pair(std::string tag, std::string label, std::string text);
		static void warning(std::string tag, std::string text, bool showTag=true);
		static void error(std::string tag, std::string text, bool showTag=true);

	private:
		static void logging(std::string tag, const char* tagColor, const char* textColor, std::string text, bool showTag=true);
};

#endif// LOG_H
