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
}  // namespace Logging

#endif