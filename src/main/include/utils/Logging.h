#ifndef LOGGING_H
#define LOGGING_H

#include <frc/smartdashboard/SmartDashboard.h>

#include <iostream>

namespace Logging {
const std::string keyPrefix = "LOG";
enum class Level { VERBOSE, INFO, WARNING, ERROR };

static void logToSmartDashboard(const std::string& key, const std::string& msg,
                                Level level = Level::INFO) {
    frc::SmartDashboard::PutString(key, msg);
}

static void logToStdOut(const std::string& key, const std::string& msg,
                        Level level = Level::VERBOSE,
                        const std::string& modifiersDontUse = "") {
    switch (level) {
        case Level::VERBOSE:
            std::cout << "VERBOSE: "
                      << "[" << key << "] " << msg << std::endl;
            break;
        case Level::INFO:
            std::cout << "INFO: "
                      << "[" << key << "] " << msg << std::endl;
            break;
        case Level::WARNING:
            std::cout << "WARNING: "
                      << "[" << key << "] " << msg << std::endl;
            break;
        case Level::ERROR:
            std::cerr << "ERROR: "
                      << "[" << key << "] " << msg << std::endl;
            break;
        default:
            std::cout << "LOG: "
                      << "[" << key << "] " << msg << std::endl;
            break;
    }
}

}  // namespace Logging

#endif