#pragma once
#include "dependencies/entt/entt.hpp"
#include "root/graphics/implementation/texture.h"
#include <vector>

class Material
{
public:
    Material(float shininess, std::vector<entt::resource<Texture>> diffuse_maps, std::vector<entt::resource<Texture>> specular_maps);

    float shininess;
    std::vector<entt::resource<Texture>> diffuse_maps;
    std::vector<entt::resource<Texture>> specular_maps;

private:
};