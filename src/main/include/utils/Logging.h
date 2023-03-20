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
                        Level level = Level::VERBOSE, const std::string& ansiKeyModifiers = "") {
    switch (level) {
        case Level::VERBOSE:
            std::cout << "\033[34;1mVERBOSE: "
                      << "[" << ansiKeyModifiers << key << "\033[34;1m]\033[34m " << msg << "\033[0m"
                      << std::endl;
            break;
        case Level::INFO:
            std::cout << "\033[32;1mINFO: "
                      << "[" << ansiKeyModifiers << key << "\033[32;1m]\033[32m " << msg << "\033[0m"
                      << std::endl;
            break;
        case Level::WARNING:
            std::cout << "\033[33;1mWARNING: "
                      << "[" << ansiKeyModifiers << key << "\033[33;1m]\033[33m" << msg << "\033[0m"
                      << std::endl;
            break;
        case Level::ERROR:
            std::cout << "\033[31;1;7mERROR: "
                      << "[" << ansiKeyModifiers << key << "\033[31;1;7m]\033[31m" << msg << "\033[0m"
                      << std::endl;
            break;
        default:
            std::cout << "\033[37mLOG: "
                      << "[" << ansiKeyModifiers << key << "\033[37m]" << msg << "\033[0m"
                      << std::endl;
            break;
    }
}

}  // namespace Logging

#endif