#include "rendering/shader/shader.h"
#include <glm/gtc/type_ptr.hpp>
#include "utils/logging/logging.h"

Shader::Shader(const char *vertex_shader_path, const char *fragment_shader_path)
{
    // load shader code
    std::string vertexShaderCode = loadShaderFile(vertex_shader_path);
    const char *vertexShaderCode_cstr = vertexShaderCode.c_str();

    std::string fragmentShaderCode = loadShaderFile(fragment_shader_path);
    const char *fragmentShaderCode_cstr = fragmentShaderCode.c_str();

    // compile shaders
    unsigned int vertexShader_id = Shader::compileShader(vertexShaderCode_cstr, GL_VERTEX_SHADER);
    unsigned int fragmentShader_id = Shader::compileShader(fragmentShaderCode_cstr, GL_FRAGMENT_SHADER);

    // create shader program
    program_ID = Shader::buildShaderProgram(vertexShader_id, fragmentShader_id);

    // delete the independent shaders (they are now integrated)
    glDeleteShader(vertexShader_id);
    glDeleteShader(fragmentShader_id);
}

Shader::Shader(Shader &&other)
{
    // obtain ownership of shader program
    this->program_ID = other.program_ID;
    // remove ownership of shader program from other
    other.program_ID = 0;
}

Shader &Shader::operator=(Shader &&other) noexcept
{
    if (this != &other)
    {
        // delete the current shader program (as we are being assigned to a new one and therefore the current must be binned)
        glDeleteProgram(program_ID);
        // obtain ownership of shader program
        this->program_ID = other.program_ID;
        // remove ownership of shader program from other
        other.program_ID = 0;
    }
    return *this;
}

Shader::~Shader()
{
    glDeleteProgram(program_ID);
}

void Shader::use()
{
    glUseProgram(program_ID);
}

void Shader::setUniform(const std::string &uniform_name, bool value) const
{
    glUniform1i(glGetUniformLocation(program_ID, uniform_name.c_str()), (int)value);
}

void Shader::setUniform(const std::string &uniform_name, int value) const
{
    glUniform1i(glGetUniformLocation(program_ID, uniform_name.c_str()), value);
}

void Shader::setUniform(const std::string &uniform_name, float value) const
{
    glUniform1f(glGetUniformLocation(program_ID, uniform_name.c_str()), value);
}

void Shader::setUniform(const std::string &uniform_name, unsigned int count, bool transpose, const glm::mat4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(program_ID, uniform_name.c_str()), count, transpose, glm::value_ptr(value));
}

void Shader::setUniform(const std::string &uniform_name, unsigned int count, bool transpose, const glm::mat3 value) const
{
    glUniformMatrix3fv(glGetUniformLocation(program_ID, uniform_name.c_str()), count, transpose, glm::value_ptr(value));
}

void Shader::setUniform(const std::string &uniform_name, const glm::vec3 value) const
{
    glUniform3f(glGetUniformLocation(program_ID, uniform_name.c_str()), value.x, value.y, value.z);
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
        LOG(std::string("Failed to read shader file: ") + shader_path, Logging::LOG_TYPE::ERROR);
    }
    return shaderCode;
}

unsigned int Shader::compileShader(const char *shader_code, GLenum shader_type)
{
    // create and compile shader
    unsigned int shader_id;
    shader_id = glCreateShader(shader_type); // init shader object in OpenGL
    glShaderSource(shader_id, 1, &shader_code, NULL); // provide object with source code
    glCompileShader(shader_id);
    // check success
    int success;
    char infoLog[512];
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader_id, 512, NULL, infoLog);
        LOG(std::string("Failed to compile shader file: \n") + infoLog, Logging::LOG_TYPE::ERROR);
    }
    return shader_id;
}

unsigned int Shader::buildShaderProgram(unsigned int vertexShader_id, unsigned int fragmentShader_id)
{
    // create shader program
    unsigned int shaderProgram_id = glCreateProgram();
    glAttachShader(shaderProgram_id, vertexShader_id);
    glAttachShader(shaderProgram_id, fragmentShader_id);
    glLinkProgram(shaderProgram_id);

    // chack for errors
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram_id, 512, NULL, infoLog);
        LOG(std::string("Failed to link shader program: \n") + infoLog, Logging::LOG_TYPE::ERROR);
    }
    return shaderProgram_id;
}
