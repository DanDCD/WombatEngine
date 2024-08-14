#include "utils/logging/logging.h"

void Logging::log(const std::string &message, LOG_TYPE log_type, const std::string &file, unsigned int line, const std::string &func)
{
    std::cout << getColor(log_type) << "[" << getLabel(log_type)
              << " in " << file << ": "
              << line << " - " << func << "]: "
              << message << Logging::RESET << std::endl;
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

std::string Logging::getLabel(LOG_TYPE log_type)
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
        return Logging::GREEN;
    }
}