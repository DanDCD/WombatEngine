#include "shader/shader.h"

Shader::Shader(const char *vertex_shader_path, const char *fragment_shader_path)
{
    const char *vertexShaderCode_cstr = loadShaderFile(vertex_shader_path).c_str();
    const char *fragmentShaderCode_cstr = loadShaderFile(fragment_shader_path).c_str();
}

void Shader::use()
{
}

void Shader::setUniform(const std::string &uniform_name, bool value)
{
}

void Shader::setUniform(const std::string &uniform_name, int value)
{
}

void Shader::setUniform(const std::string &uniform_name, float value)
{
}

std::string Shader::loadShaderFile(const char *shader_path)
{

    std::string shaderCode;
    std::ifstream shaderFile;
    // ensure that if the fail/bad bits are set, we throw an exception of type ifstream::failure
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    // read from the file
    try
    {
        // open files
        shaderFile.open(shader_path);
        std::stringstream shaderStream;
        // read file's buffer contents into streams
        shaderStream << shaderFile.rdbuf();
        // close file handlers
        shaderFile.close();
        // convert string stream into a string
        shaderCode = shaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY READ: " << shader_path << std::endl;
    }
    return shaderCode;
}
