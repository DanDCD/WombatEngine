#pragma once

#include <vector>
#include <tuple>
#include <glad/glad.h>
#include <string>
#include <glm/vec2.hpp>
#include <iostream>
#include <stb/stb_image.h>

struct TextureParam
{
public:
    /// @brief the name of the texture parameter to update (i.e. GL_TEXTURE_WRAP_S)
    GLenum paramName;

    /// @brief the value to assign to the parameter (i.e. GL_REPEAT)
    GLenum value;

    /// @brief constructor
    /// @param paramName \copydoc paramName
    /// @param value \copydoc value
    TextureParam(GLenum paramName, GLenum value);

    /// @brief tuple constructor
    /// @param pair tuple containing \copydoc paramName and \copydoc value
    TextureParam(std::tuple<GLenum, GLenum> pair);
};

class Texture
{
public:
    /// @brief constructor - creates the texture object in OpenGL
    /// @param textureTargetType \copydoc textureTargetType
    /// @param params a vector of OpenGL texture options to apply to this texture object
    /// @param texture_path the path to the image file from which the texture data will be loaded
    /// @param texture_unit the OpenGL texture unit that this texture will be assigned to and accessed via a Sampler in a shader etc.
    Texture(GLenum texture_target_type, const std::vector<TextureParam> &params, const std::string &texture_path, GLenum texture_unit);

    /// @brief constructor - creates the texture object in OpenGL
    /// @param textureTargetType \copydoc textureTargetType
    /// @param params a vector of OpenGL texture options to apply to this texture object
    /// @param texture_path the path to the image file from which the texture data will be loaded
    Texture(GLenum texture_target_type, const std::vector<TextureParam> &params, const std::string &texture_path);

    /// @brief destructor - deletes texture from OpenGL
    ~Texture();

    /// @brief bind the texture to active state in OpenGL
    void bind();

    /// @brief unbind the texture from active state in OpenGL
    void unbind();

private:
    /// @brief the id of the texture object in OpenGL
    unsigned int texture_ID;

    /// @brief the type of texture (i.e. GL_TEXTURE_2D)
    GLenum textureTargetType;

    /// @brief the texture unit to be associated with this texture (i.e. GL_TEXTURE1)
    GLenum textureUnit;

    /// @brief the height/width of the texture
    glm::vec2 dimensions;

    /// @brief load and apply an image file as the texture data for this texture (only accepts png and jpg)
    /// @param texture_path the path to the file containing texture data
    void assignTexture(const std::string &texture_path);
};