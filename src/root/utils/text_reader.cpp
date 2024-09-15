#include "root/utils/text_reading.h"
#include "fstream"
#include "sstream"

std::string readFile(const std::string &file_path)
{
    std::ifstream file(file_path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}