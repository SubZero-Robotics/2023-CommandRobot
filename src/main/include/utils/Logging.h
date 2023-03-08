#ifndef LOGGING_H
#define LOGGING_H

#include <iostream>
#include <format>
#include <frc/smartdashboard/SmartDashboard.h>

namespace Logging {
    enum class Level {
        VERBOSE,
        INFO,
        WARNING,
        ERROR
    };

    static void logToSmartDashboard(const std::string & key, const std::string & msg, Level level) {
        frc::SmartDashboard::PutString(key, getLevelString(msg, level));
    }

    static void logToStdOut(const std::string & key, const std::string & msg, Level level) {
        std::cout << getLevelString(msg, level);
    }

    static std::string getLevelString(const std::string & msg, Level level) {
        switch (level) {
            case Level::VERBOSE:
                return std::format("VERBOSE: {}\n", msg);
            case Level::INFO:
                return std::format("INFO: {}\n", msg);
            case Level::WARNING:
                return std::format("WARNING: {}\n", msg);
            case Level::ERROR:
                return std::format("ERROR: {}\n", msg);
            default:
                return std::format("LOG: {}\n", msg);
        }
    }
}

#endif