#include "utils/text_reading/text_reading.h"
#include "fstream"
#include "sstream"

std::string readFile(const std::string &file_path)
{
    std::ifstream file(file_path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}