#pragma once
#include <string>
#include <iostream>

namespace Logging
{

    // ANSI escape codes for colors
    const std::string RESET = "\033[0m";
    const std::string RED = "\033[31m";
    const std::string YELLOW = "\033[33m";
    const std::string GREEN = "\033[32m";

    /// @brief an enum used to define certain types of logging messages
    enum class LOG_TYPE
    {
        INFO,
        ERROR,
        WARNING
    };

    /// @brief log a message to output
    /// @param message the message to log
    /// @param log_type the type/severity of the log message
    /// @param file the file this log was sent from
    /// @param line the line this log was sent from
    /// @param func the function this log was sent from
    void log(const std::string &message, LOG_TYPE log_type, const std::string &file, unsigned int line, const std::string &func);

// definition macro enables us to capture the location of the logging message
#define LOG(message, log_type) Logging::log(message, log_type, __FILE__, __LINE__, __func__)

    /// @brief return the associated color for the given log severity
    /// @param log_type the type of log
    /// @return an ANSI formatted color
    static std::string getColor(LOG_TYPE log_type);

    /// @brief get the label for a given log type
    /// @param log_type the log type
    /// @return a string label representing the log type
    static std::string getLabel(LOG_TYPE log_type);
}