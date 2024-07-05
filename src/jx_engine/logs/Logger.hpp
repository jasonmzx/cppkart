#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <string>

class Logger {
    public:

        static Logger* instance;

        enum LogLevel {
            INFO,
            WARN,
            ERROR,
            TIMER,
            FATAL
        };

        static Logger* getInstance();

        void log(LogLevel level, const std::string& message);
        void enableLogging();
        void disableLogging();
    
    private:
        bool loggingEnabled = true;
        Logger() {}
        std::string getLogLevelColor(LogLevel level);

};

#endif