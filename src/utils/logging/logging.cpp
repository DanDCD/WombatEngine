#include "utils/logging/logging.h"

void Logging::log(const std::string &message, LOG_TYPE log_type, LOG_PRIORITY log_priority, const std::string &file, unsigned int line, const std::string &func)
{
    if (log_priority < min_allowed_priority && log_type != LOG_TYPE::ERROR) // dismiss message unless its above threshold or is an error
        return;
    std::cout << getColor(log_type) << "[" << getTypeLabel(log_type)
              << " in " << file << ": "
              << line << " - " << func << "]: "
              << message << Logging::RESET << std::endl;
}

void Logging::set_minimum_priority(LOG_PRIORITY new_priority)
{
    min_allowed_priority = new_priority;
    LOG("New minimum priority set for logging: " + getPriorityLabel(new_priority), LOG_TYPE::INFO, LOG_PRIORITY::HIGH);
}

Logging::LOG_PRIORITY Logging::get_minimum_priority()
{
    return min_allowed_priority;
}

std::string Logging::getColor(LOG_TYPE log_type)
{
    switch (log_type)
    {
    case LOG_TYPE::ERROR:
        return Logging::RED;
    case LOG_TYPE::WARNING:
        return Logging::YELLOW;
    case LOG_TYPE::INFO:
        return Logging::GREEN;
    default:
        return Logging::GREEN;
    }
}

std::string Logging::getTypeLabel(LOG_TYPE log_type)
{
    switch (log_type)
    {
    case LOG_TYPE::ERROR:
        return "ERROR";
    case LOG_TYPE::WARNING:
        return "WARNING";
    case LOG_TYPE::INFO:
        return "INFO";
    default:
        return "INFO";
    }
}


std::string Logging::getPriorityLabel(LOG_PRIORITY log_type)
{
    switch (log_type)
    {
    case LOG_PRIORITY::HIGH:
        return "HIGH";
    case LOG_PRIORITY::MEDIUM:
        return "MEDIUM";
    case LOG_PRIORITY::LOW:
        return "LOW";
    default:
        return "LOW";
    }
}