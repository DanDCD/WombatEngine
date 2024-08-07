#pragma once
#include <unordered_map>
#include "rendering/texture/texture.h"
#include <string>

class TextureManager
{
public:

private:
    TextureManager();

    std::unordered_map<std::string, Texture> locationToTexture;

};