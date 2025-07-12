#include "LogSystem.h"

LogSystem::LogSystem() {

}

LogSystem::~LogSystem()
{
	if (m_LogFile.is_open()) {
		log(LogLevel::LOG_LEVEL_INFO, "Log file closed: " + m_logFilePath + m_logObject);
		m_LogFile.close();
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
	m_logToFile = logToFile;
	m_logFilePath = logFilePath;
	m_logObject = logObject;
	std::string logTime = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
	std::string logname = logObject + "_" + logTime;

	if (m_logToFile == LOG_ENABLE) {
		m_LogFile.open(logFilePath + logname + ".log");
		if (m_LogFile.is_open()) {
			log(LogLevel::LOG_LEVEL_INFO, "Log file created: " + logFilePath + logname + ".log");
		}
		else {
			log(LogLevel::LOG_LEVEL_FATAL, "Failed to create log file: " + logFilePath + logname + ".log");
		}
	}
}

void LogSystem::log(const LogLevel& logLevel, const std::string& message)
{
	std::time_t rawTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm resultTime{};
    localtime_s(&resultTime, &rawTime);
	if (m_logToFile && m_LogFile.is_open()) {
		switch (logLevel) {
		case LogLevel::LOG_LEVEL_INFO:
			m_LogFile << TIME << "[INFO] " << message << std::endl;
			break;
		case LogLevel::LOG_LEVEL_WARN:
			m_LogFile << TIME << "[WARNING] " << message << std::endl;
			break;
		case LogLevel::LOG_LEVEL_ERROR:
			m_LogFile << TIME << "[ERROR] " << message << std::endl;
			break;
		case LogLevel::LOG_LEVEL_FATAL:
			m_LogFile << TIME << "[FATAL] " << message << std::endl;
			break;
		case LogLevel::LOG_LEVEL_DEBUG:
			m_LogFile << TIME << "[DEBUG] " << message << std::endl;
			break;
		default:
			break;
		}
	}
	switch (logLevel) {
	case LogLevel::LOG_LEVEL_INFO:
		std::cout << TIME << DEFAULT << "[INFO] " << message << std::endl;
		break;
	case LogLevel::LOG_LEVEL_WARN:
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
	if (m_logToFile && m_LogFile.is_open()) {
		m_LogFile << TIME << "[" << customLogLevel << "] " << message << std::endl;
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


