#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{

public:
    /// @brief the id of this Shader Object's shader program in OpenGL
    unsigned int program_ID;

    // constructer that reads shader code and builds the shader program
    Shader(const char *vertex_shader_path, const char *fragment_shader_path);

    // destructor that destroys shader program in OpenGL
    


    /// @brief tell OpenGL to use this shader
    void use();

    /// @brief set the value of a uniform in the shader program
    /// @param uniform_name
    /// @param value
    void setUniform(const std::string &uniform_name, bool value) const;

    /// @brief set the value of a uniform in the shader program
    /// @param uniform_name
    /// @param value
    void setUniform(const std::string &uniform_name, int value) const;

    /// @brief set the value of a uniform in the shader program
    /// @param uniform_name
    /// @param value
    void setUniform(const std::string &uniform_name, float value) const;

private:
    /// @brief load a shader file and return its source code
    /// @param shader_path the location of the shader file
    /// @return the source code of the shader file
    static std::string loadShaderFile(const char *shader_path);

    /// @brief compiles shader source code and creates a shader object in OpenGL
    /// @param shader_code the shader source code to be compiled
    /// @param shader_type the type of shader: i.e. GL_VERTEX_SHADER
    /// @return the identifier of the shader object in OpenGL
    static unsigned int compileShader(const char *shader_code, GLenum shader_type);

    /// @brief create a shader program in OpenGL, using the vertex and fragment shader objects
    /// @param vertexShader_id the id of the vertex shader in OpenGL
    /// @param fragmentShader_id the id of the fragment shader in OpenGL
    /// @return the id of the shader object in OpenGL
    static unsigned int buildShaderProgram(unsigned int vertexShader_id, unsigned int fragmentShader_id);
};