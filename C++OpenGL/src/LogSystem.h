
#pragma once

#include "LogSystem.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>


#define COLORED 1
#define COLORLESS 0
#define FAILED 0
#define SUCCESS 1
#define ENABLE 1
#define DISABLE 0

#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RED "\033[31m"
#define DEFAULT "\033[0m"
#define BLUE "\033[34m"

#define TIME std::put_time(&resultTime, "[%H:%M:%S]")

using status = int;

class LogSystem {
private:
	std::ofstream LogFile;
	bool logToFile = true;


	status initLogFile(const std::string& logFilePath, const std::string& logFileName);
	status shutdown();

public:

	enum class LogLevel {
		LOG_LEVEL_INFO,
		LOG_LEVEL_WARNING,
		LOG_LEVEL_ERROR,
		LOG_LEVEL_FATAL,
		LOG_LEVEL_DEBUG,
	};
	enum class LogColor {
		LOG_COLOR_GREEN,
		LOG_COLOR_YELLOW,
		LOG_COLOR_RED,
		LOG_COLOR_BLUE,
		LOG_COLOR_DEFAULT,
	};

	LogSystem(const std::string& logFilePath, const std::string& logFileName, const bool& logToFile)
		: logToFile(logToFile){
		if (logToFile)
			initLogFile(logFilePath, logFileName);
	}

	~LogSystem() {
		shutdown();
	}

	void LogCustom(const std::string& customLogLevel, const LogColor& logColor, const std::string& message);

	void log(const LogLevel& logLevel, const std::string& message);
};