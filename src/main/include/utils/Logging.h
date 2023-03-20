#ifndef LOGGING_H
#define LOGGING_H

#include <frc/smartdashboard/SmartDashboard.h>

#include <iostream>

namespace Logging {
const std::string keyPrefix = "LOG";
enum class Level { VERBOSE, INFO, WARNING, ERROR };

static std::string getLevelString(const std::string& msg, Level level);

static void logToSmartDashboard(const std::string& key, const std::string& msg,
                                Level level = Level::INFO) {
    frc::SmartDashboard::PutString(keyPrefix + "-" + key, msg);
}

static void logToStdOut(const std::string& key, const std::string& msg,
                        Level level) {
    switch (level) {
        case Level::VERBOSE:
            std::cout << "\033[34;1m"
                      << "[" << key << "]\033[34m VERBOSE: " << msg << "\033[0m"
                      << std::endl;
        case Level::INFO:
            std::cout << "\033[32;1m"
                      << "[" << key << "]\033[32m INFO: " << msg << "\033[0m"
                      << std::endl;
        case Level::WARNING:
            std::cout << "\033[33;1m"
                      << "[" << key << "]\033[33m WARNING: " << msg << "\033[0m"
                      << std::endl;
        case Level::ERROR:
            std::cout << "\033[31;1;7m"
                      << "[" << key << "]\033[31m ERROR: " << msg << "\033[0m"
                      << std::endl;
        default:
            std::cout << "\033[37m"
                      << "[" << key << "] LOG: " << msg << "\033[0m"
                      << std::endl;
    }
}

}  // namespace Logging

#endif