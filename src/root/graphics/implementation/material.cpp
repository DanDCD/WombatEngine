#include "root/graphics/implementation/material.h"

Material::Material(float shininess, std::vector<entt::resource<Texture>> diffuse_maps, std::vector<entt::resource<Texture>> specular_maps)
    : shininess(shininess), diffuse_maps(diffuse_maps), specular_maps(specular_maps)
{
}

std::vector<entt::resource<Texture>> Material::getTexturesFlat() const
{
    std::vector<entt::resource<Texture>> flat_vec;
    flat_vec.reserve(diffuse_maps.size() + specular_maps.size());
    flat_vec.insert(flat_vec.end(), diffuse_maps.begin(), diffuse_maps.end());
    flat_vec.insert(flat_vec.end(), specular_maps.begin(), specular_maps.end());
    return flat_vec;
}