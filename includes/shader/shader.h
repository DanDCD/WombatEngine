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

    // activate the shader
    void use();
    
    // utility functions for uniforms
    void setUniform(const std::string &uniform_name, bool value);
    void setUniform(const std::string &uniform_name, int value);
    void setUniform(const std::string &uniform_name, float value);

private:
    static std::string loadShaderFile(const char* shader_path);

};