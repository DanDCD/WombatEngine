#include "rendering/texture/texture.h"
#include "utils/logging/logging.h"

TextureParam::TextureParam(GLenum paramName, GLenum value)
{
    this->paramName = paramName;
    this->value = value;
}

TextureParam::TextureParam(std::tuple<GLenum, GLenum> pair)
{
    this->paramName = std::get<0>(pair);
    this->value = std::get<1>(pair);
}

Texture::Texture(GLenum texture_target_type, const std::vector<TextureParam> &params, const std::string &texture_path, GLenum texture_unit)
{
    // generate a texture object in OpenGL
    unsigned int id;
    glGenTextures(1, &id);
    this->texture_ID = id;

    // bind it (using the explicit texture type)
    this->textureTargetType = texture_target_type;
    glBindTexture(this->textureTargetType, this->texture_ID);

    this->textureUnit = texture_unit;

    // set the parameters (if any)
    for (const auto &param : params)
    {
        glTexParameteri(this->textureTargetType, param.paramName, param.value);
    }

    // load and apply the texture
    assignTexture(texture_path);

    // unbind the texture after set up to maintain a clean state
    unbind();
}

Texture::Texture(GLenum texture_target_type, const std::vector<TextureParam> &params, const std::string &texture_path)
    : Texture::Texture(texture_target_type, params, texture_path, GL_TEXTURE0)
{
}

Texture::Texture(Texture &&other)
{
    // copy over important params
    this->texture_ID = other.texture_ID; // obtain ownership of texture object
    this->textureTargetType = other.textureTargetType;
    this->textureUnit = other.textureUnit;
    this->dimensions = other.dimensions;
    // remove ownership of the texture object from other
    other.texture_ID = 0;
}

Texture &Texture::operator=(Texture &&other) noexcept
{
    if (this != &other)
    {
        // delete current texture
        glDeleteTextures(1, &texture_ID);
        // copy over important params
        this->texture_ID = other.texture_ID; // obtain ownership of texture object
        this->textureTargetType = other.textureTargetType;
        this->textureUnit = other.textureUnit;
        this->dimensions = other.dimensions;
        // remove ownership of the texture object from other
        other.texture_ID = 0;
    }
    return *this;
}

Texture::~Texture()
{
    glDeleteTextures(1, &texture_ID); // delete the associated texture object from OpenGL
}

void Texture::bind()
{
    glActiveTexture(this->textureUnit);                       // activate the associated texture unit
    glBindTexture(this->textureTargetType, this->texture_ID); // bind this texture to the unit
}

void Texture::assignTexture(const std::string &texture_path)
{
    LOG("Attempting to load texture from: " + texture_path, Logging::LOG_TYPE::INFO, Logging::LOG_PRIORITY::MEDIUM);
    // load texture
    int width, height, nrChannels;
    unsigned char *texture_data = stbi_load(texture_path.c_str(), &width, &height, &nrChannels, 0);
    if (texture_data)
    {
        // determine underlying format of texture file
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        if (nrChannels == 3)
            format = GL_RGB;
        if (nrChannels == 4)
            format = GL_RGBA;
        // bind the texture (should already be bound but just in case)
        glBindTexture(this->textureTargetType, this->texture_ID);
        // bind texture data to the currently bound texture object
        glTexImage2D(this->textureTargetType, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texture_data);
        // generate mip maps for the texture (smaller textures for distant renders)
        glGenerateMipmap(this->textureTargetType);
        // set dimensions
        this->dimensions = glm::vec2(width, height);
        LOG("Successfuly loaded texture from: " + texture_path, Logging::LOG_TYPE::INFO, Logging::LOG_PRIORITY::MEDIUM);
    }
    else
    {
        LOG("Failed to load texture file: " + texture_path, Logging::LOG_TYPE::ERROR);
    }

    stbi_image_free(texture_data);
}

void Texture::unbind()
{
    glBindTexture(this->textureTargetType, 0);
}