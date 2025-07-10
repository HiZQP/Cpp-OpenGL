#include "LogSystem.h"

status LogSystem::initLogFile(const std::string& logFilePath, const std::string& logFileName)
{
	std::string logTime = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
	std::string logname = logFileName + "_" + logTime;

	LogFile.open(logFilePath + logname + ".log");

	if (!LogFile.is_open()) {
		log(LogLevel::LOG_LEVEL_FATAL, "Failed to create log file: " + logFilePath + logname + ".log");
		return FAILED;
	}

	log(LogLevel::LOG_LEVEL_INFO, "Log file created: " + logFilePath + logname + ".log");
	return SUCCESS;
}

status LogSystem::shutdown()
{
	if (LogFile.is_open()) {
		LogFile.close();
	}
	return SUCCESS;
}

void LogSystem::log(const LogLevel& logLevel, const std::string& message)
{
	std::time_t rawTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm resultTime{};
    localtime_s(&resultTime, &rawTime);
	if (logToFile && LogFile.is_open()) {
		switch (logLevel) {
		case LogLevel::LOG_LEVEL_INFO:
			LogFile << TIME << "[INFO] " << message << std::endl;
			break;
		case LogLevel::LOG_LEVEL_WARNING:
			LogFile << TIME << "[WARNING] " << message << std::endl;
			break;
		case LogLevel::LOG_LEVEL_ERROR:
			LogFile << TIME << "[ERROR] " << message << std::endl;
			break;
		case LogLevel::LOG_LEVEL_FATAL:
			LogFile << TIME << "[FATAL] " << message << std::endl;
			break;
		case LogLevel::LOG_LEVEL_DEBUG:
			LogFile << TIME << "[DEBUG] " << message << std::endl;
			break;
		default:
			break;
		}
	}
	switch (logLevel) {
	case LogLevel::LOG_LEVEL_INFO:
		std::cout << TIME << DEFAULT << "[INFO] " << message << std::endl;
		break;
	case LogLevel::LOG_LEVEL_WARNING:
		std::cout << TIME << YELLOW << "[WARNING] " << message << DEFAULT << std::endl;
		break;
	case LogLevel::LOG_LEVEL_ERROR:
		std::cout << TIME << RED << "[ERROR] " << message << DEFAULT << std::endl;
		break;
	case LogLevel::LOG_LEVEL_FATAL:
		std::cout << TIME << RED << "[FATAL] " << message << DEFAULT << std::endl;
		break;
	case LogLevel::LOG_LEVEL_DEBUG:
		std::cout << TIME << BLUE << "[DEBUG] " << message << DEFAULT << std::endl;
		break;
	default:
		break;
	}
}

void LogSystem::LogCustom(const std::string& customLogLevel, const LogColor& logColor, const std::string& message)
{
	std::time_t rawTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm resultTime{};
	localtime_s(&resultTime, &rawTime);
	if (logToFile && LogFile.is_open()) {
		LogFile << TIME << "[" << customLogLevel << "] " << message << std::endl;
	}
	switch (logColor) {
	case LogColor::LOG_COLOR_GREEN:
		std::cout << TIME << GREEN << "[" << customLogLevel << "] " << message << DEFAULT << std::endl;
		break;
	case LogColor::LOG_COLOR_YELLOW:
		std::cout << TIME << YELLOW << "[" << customLogLevel << "] " << message << DEFAULT << std::endl;
		break;
	case LogColor::LOG_COLOR_RED:
		std::cout << TIME << RED << "[" << customLogLevel << "] " << message << DEFAULT << std::endl;
		break;
	case LogColor::LOG_COLOR_BLUE:
		std::cout << TIME << BLUE << "[" << customLogLevel << "] " << message << DEFAULT << std::endl;
		break;
	case LogColor::LOG_COLOR_DEFAULT:
		std::cout << TIME << DEFAULT << "[" << customLogLevel << "] " << message << std::endl;
		break;
	default:
		break;
	}
}


