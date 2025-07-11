#include "LogSystem.h"

LogSystem::LogSystem() {

}

LogSystem::~LogSystem()
{
	if (LogFile.is_open()) {
		log(LogLevel::LOG_LEVEL_INFO, "Log file closed: " + logFilePath + logObject);
		LogFile.close();
	}
}

LogSystem& LogSystem::getInstance()
{
	// 使用单例模式，确保只有一个 LogSystem 实例
	static LogSystem instance;
	return instance;
}

void LogSystem::init(const std::string& logFilePath, const std::string& logObject, const bool& logToFile)
{
	this->logToFile = logToFile;
	this->logFilePath = logFilePath;
	this->logObject = logObject;
	std::string logTime = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
	std::string logname = logObject + "_" + logTime;

	LogFile.open(logFilePath + logname + ".log");

	if (!LogFile.is_open()) {
		log(LogLevel::LOG_LEVEL_FATAL, "Failed to create log file: " + logFilePath + logname + ".log");
	}

	log(LogLevel::LOG_LEVEL_INFO, "Log file created: " + logFilePath + logname + ".log");
}

void LogSystem::log(const LogLevel& logLevel, const std::string& message)
{
	std::time_t rawTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm resultTime{};
    localtime_s(&resultTime, &rawTime);
	if (logToFile && LogFile.is_open()) {
		switch (logLevel) {
		case LogLevel::LOG_LEVEL_INFO:
			LogFile << TIME << "[INFO] " << OBJECT << message << std::endl;
			break;
		case LogLevel::LOG_LEVEL_WARN:
			LogFile << TIME << "[WARNING] " << OBJECT << message << std::endl;
			break;
		case LogLevel::LOG_LEVEL_ERROR:
			LogFile << TIME << "[ERROR] " << OBJECT << message << std::endl;
			break;
		case LogLevel::LOG_LEVEL_FATAL:
			LogFile << TIME << "[FATAL] " << OBJECT << message << std::endl;
			break;
		case LogLevel::LOG_LEVEL_DEBUG:
			LogFile << TIME << "[DEBUG] " << OBJECT << message << std::endl;
			break;
		default:
			break;
		}
	}
	switch (logLevel) {
	case LogLevel::LOG_LEVEL_INFO:
		std::cout << TIME << DEFAULT << "[INFO] " << OBJECT << message << std::endl;
		break;
	case LogLevel::LOG_LEVEL_WARN:
		std::cout << TIME << YELLOW << "[WARNING] " << OBJECT << message << DEFAULT << std::endl;
		break;
	case LogLevel::LOG_LEVEL_ERROR:
		std::cout << TIME << RED << "[ERROR] " << OBJECT << message << DEFAULT << std::endl;
		break;
	case LogLevel::LOG_LEVEL_FATAL:
		std::cout << TIME << RED << "[FATAL] " << OBJECT << message << DEFAULT << std::endl;
		break;
	case LogLevel::LOG_LEVEL_DEBUG:
		std::cout << TIME << BLUE << "[DEBUG] " << OBJECT << message << DEFAULT << std::endl;
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


