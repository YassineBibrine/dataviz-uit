#include "logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

Logger &Logger::getInstance()
{
    static Logger instance;
    return instance;
}

Logger::~Logger()
{
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::setLogFile(const std::string &filePath)
{
    if (logFile.is_open()) {
        logFile.close();
    }
    logFile.open(filePath, std::ios_base::app);
}

void Logger::log(LogLevel level, const std::string &message)
{
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    
    std::string output = "[" + oss.str() + "] [" + levelToString(level) + "] " + message;
    
    std::cout << output << std::endl;
    if (logFile.is_open()) {
        logFile << output << std::endl;
 logFile.flush();
    }
}

void Logger::debug(const std::string &message)
{
    log(LogLevel::DEBUG, message);
}

void Logger::info(const std::string &message)
{
    log(LogLevel::INFO, message);
}

void Logger::warning(const std::string &message)
{
    log(LogLevel::WARNING, message);
}

void Logger::error(const std::string &message)
{
    log(LogLevel::ERROR, message);
}

void Logger::critical(const std::string &message)
{
    log(LogLevel::CRITICAL, message);
}

std::string Logger::levelToString(LogLevel level) const
{
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::CRITICAL: return "CRITICAL";
     default: return "UNKNOWN";
    }
}
