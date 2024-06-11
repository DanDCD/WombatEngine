# pragma once

#include <glad/glad.h>
  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader {
    
public:
    
    unsigned int program_ID;

    // constructer that reads shader code and builds the shader program
    Shader(const char* vertex_shader_path, const char* fragment_shader_path);

  
    /// @brief 
    void use();
    

    /// @brief 
    /// @param uniform_name 
    /// @param value 
    void setUniform(const std::string &uniform_name, bool value);

    /// @brief 
    /// @param uniform_name 
    /// @param value 
    void setUniform(const std::string &uniform_name, int value);

    /// @brief 
    /// @param uniform_name 
    /// @param value 
    void setUniform(const std::string &uniform_name, float value);

private:
    /// @brief load a shader file and return its source code
    /// @param shader_path the location of the shader file
    /// @return the source code of the shader file
    static std::string loadShaderFile(const char* shader_path);

    /// @brief compiles shader source code and creates a shader object in OpenGL
    /// @param shader_code the shader source code to be compiled
    /// @param shader_type the type of shader: i.e. GL_VERTEX_SHADER
    /// @return the identifier of the shader object in OpenGL
    static unsigned int compileShader(const char* shader_code, GLenum shader_type);

};