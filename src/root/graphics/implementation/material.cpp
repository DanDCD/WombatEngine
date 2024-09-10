#include "root/graphics/implementation/material.h"

Material::Material(float shininess, std::vector<entt::resource<Texture>> diffuse_maps, std::vector<entt::resource<Texture>> specular_maps)
    : shininess(shininess), diffuse_maps(diffuse_maps), specular_maps(specular_maps)
{
}