#ifndef LOGGING_H
#define LOGGING_H

#include <frc/smartdashboard/SmartDashboard.h>

#include <iostream>

namespace Logging {
const std::string keyPrefix = "LOG";
enum class Level { VERBOSE, INFO, WARNING, ERROR };

static std::string getLevelString(const std::string& msg, Level level);

static void logToSmartDashboard(const std::string& key, const std::string& msg,
                                Level level) {
    frc::SmartDashboard::PutString(keyPrefix + "-" + key, msg);
}

static void logToStdOut(const std::string& key, const std::string& msg,
                        Level level) {
    std::cout << key << " - " << getLevelString(msg, level);
}

static std::string getLevelString(const std::string& msg, Level level) {
    switch (level) {
        case Level::VERBOSE:
            return "VERBOSE: " + msg + "\n";
        case Level::INFO:
            return "INFO: " + msg + "\n";
        case Level::WARNING:
            return "WARNING: " + msg + "\n";
        case Level::ERROR:
            return "ERROR: " + msg + "\n";
        default:
            return "LOG: " + msg + "\n";
    }
}

static void logPretty(const std::string& msg, Level level, const std::string& key) {
    switch (level) {
        case Level::VERBOSE:
            std::cout << "\033[1;34m" << "[" << key << "] VERBOSE: " << msg << "\033[0m" << std::endl;
        case Level::INFO:
            std::cout << "\033[1;32m" << "[" << key << "] INFO: " << msg << "\033[0m" << std::endl;
        case Level::WARNING:
            std::cout << "\033[1;33m" << "[" << key << "] WARNING: " << msg << "\033[0m" << std::endl;
        case Level::ERROR:
            std::cout << "\033[1;31m" << "[" << key << "] ERROR: " << msg << "\033[0m" << std::endl;
        default:
            std::cout << "\033[1;37m" << "[" << key << "] LOG: " << msg << "\033[0m" << std::endl;
    }
}

}  // namespace Logging

#endif