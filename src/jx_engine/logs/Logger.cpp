#include "Logger.hpp"

Logger* Logger::instance = nullptr;


void Logger::log(LogLevel level, const std::string& message) {
    if (loggingEnabled) {
        switch (level) {
            case INFO: std::cout << getLogLevelColor(INFO)   << "[INFO] " << message << "\033[0m"<< std::endl; break;
            case WARN: std::cout << getLogLevelColor(WARN)   << "[WARN] " << message << "\033[0m"<< std::endl; break;
            case ERROR: std::cout << getLogLevelColor(ERROR) << "[ERROR] " << message << "\033[0m"<< std::endl; break;
            case TIMER: std::cout << getLogLevelColor(TIMER)  << "[TIMER] " << message << "\033[0m"<< std::endl; break;
            case FATAL: std::cout << getLogLevelColor(ERROR) << "[FATAL] " << message << "\033[0m"<< std::endl; break;
        }
    }
}

std::string Logger::getLogLevelColor(LogLevel level) {
    switch (level) {
        case INFO: return "\033[33m";  // Yellow
        case WARN: return "\033[35m";  // Magenta
        case ERROR: return "\033[31m"; // Red
        case TIMER: return "\033[32m"; // Green
        default: return "\033[0m";     // Reset
    }
}

Logger* Logger::getInstance() {
    if (!instance)
        instance = new Logger;
    return instance;
}

void Logger::enableLogging() {
    loggingEnabled = true;
}

void Logger::disableLogging() {
    loggingEnabled = false;
}