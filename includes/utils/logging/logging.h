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

    /// @brief an enum used to define how important this message is to display - avoids too many verbose warnings
    enum class LOG_PRIORITY
    {
        LOW = 0,
        MEDIUM = 1,
        HIGH = 2
    };

    /// @brief log a message to output
    /// @param message the message to log
    /// @param log_type the type/severity of the log message
    /// @param log_priority how important it is to display this message
    /// @param file the file this log was sent from
    /// @param line the line this log was sent from
    /// @param func the function this log was sent from
    void log(const std::string &message, LOG_TYPE log_type, LOG_PRIORITY log_priority, const std::string &file, unsigned int line, const std::string &func);

// definition macro enables us to capture the location of the logging message


// macro to allow filename, line, and function to be automatically included in log
#define LOG(...) LOG_SELEC(__VA_ARGS__, LOG_WITH_PRIORITY, LOG_WITHOUT_PRIORITY)(__VA_ARGS__)

// Implementations of log macro
#define LOG_WITHOUT_PRIORITY(message, log_type) Logging::log(message, log_type, Logging::LOG_PRIORITY::LOW, __FILE__, __LINE__, __func__)
#define LOG_WITH_PRIORITY(message, log_type, log_priority) Logging::log(message, log_type, log_priority, __FILE__, __LINE__, __func__)

// macro used to choose between using the short LOG where a default priority is given, or the longer LOG where priority is passed by user
#define LOG_SELEC(_1, _2, _3, NAME, ...) NAME


    /// @brief update the lowest priority level for a message to be displayed
    /// @param new_priority the minimum priority level required for a log to display
    void set_minimum_priority(LOG_PRIORITY new_priority);

    /// @brief get the lowest priority level for messages to be displayed
    /// @return the lowest message priority
    LOG_PRIORITY get_minimum_priority();

    /// @brief return the associated color for the given log severity
    /// @param log_type the type of log
    /// @return an ANSI formatted color
    static std::string getColor(LOG_TYPE log_type);

    /// @brief get the label for a given log type
    /// @param log_type the log type
    /// @return a string label representing the log type
    static std::string getTypeLabel(LOG_TYPE log_type);

    /// @brief get the label for a given log priority 
    /// @param log_priority the log priority
    /// @return a string label representing the log priority
    static std::string getPriorityLabel(LOG_PRIORITY log_priority);

    /// @brief the minimum priority of messages allowed to print - messages with a lower priority will not be displayed
    static LOG_PRIORITY min_allowed_priority = LOG_PRIORITY::LOW;
}