#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

/**
 * @class Logger
 * @brief Application-wide logging utility
 * 
 * Provides centralized logging for:
 * - Debug information
 * - Warnings and errors
 * - Performance metrics
 * - File-based log persistence
 */
class Logger {
public:
    enum class LogLevel {
DEBUG,
        INFO,
        WARNING,
        ERROR,
        CRITICAL
    };
  
    static Logger &getInstance();
    
    void setLogFile(const std::string &filePath);
    void log(LogLevel level, const std::string &message);
  void debug(const std::string &message);
    void info(const std::string &message);
    void warning(const std::string &message);
    void error(const std::string &message);
    void critical(const std::string &message);

private:
    Logger() = default;
    ~Logger();
    
    std::ofstream logFile;
std::string levelToString(LogLevel level) const;
};

#endif // LOGGER_H
