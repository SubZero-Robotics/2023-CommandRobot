#ifndef LOGGING_H
#define LOGGING_H

#include <frc/smartdashboard/SmartDashboard.h>

#include <iostream>

#include "Constants.h"

namespace Logging {
const std::string keyPrefix = "LOG";

static void logToSmartDashboard(const std::string& key, const std::string& msg,
                                Level level = Level::INFO) {
    if ((int)level < (int)MinLogLevel) return;
    frc::SmartDashboard::PutString(key, msg);
}

static void logToStdOut(const std::string& key, const std::string& msg,
                        Level level = Level::VERBOSE,
                        const std::string& modifiersDontUse = "") {
    if ((int)level < (int)MinLogLevel) return;
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