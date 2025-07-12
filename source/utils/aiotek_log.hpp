#ifndef __AIOTEK_LOG_HPP__
#define __AIOTEK_LOG_HPP__

#include <string>
#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>

namespace AIOTEK {

enum class LogLevel {
    DEBUG = 0,
    INFO,
    WARNING,
    ERROR
};

class Logger {
private:
    static LogLevel currentLevel;
    static std::string getCurrentTime();
    static std::string getLevelString(LogLevel level);
    
public:
    static void setLevel(LogLevel level);
    static void log(LogLevel level, const std::string& message);
    
    static void debug(const std::string& message);
    static void info(const std::string& message);
    static void warning(const std::string& message);
    static void error(const std::string& message);
};

#define AIOTEK_LOG_DEBUG(msg) AIOTEK::Logger::debug(msg)
#define AIOTEK_LOG_INFO(msg) AIOTEK::Logger::info(msg)
#define AIOTEK_LOG_WARNING(msg) AIOTEK::Logger::warning(msg)
#define AIOTEK_LOG_ERROR(msg) AIOTEK::Logger::error(msg)

} // namespace AIOTEK

#endif /* __AIOTEK_LOG_HPP__ */
