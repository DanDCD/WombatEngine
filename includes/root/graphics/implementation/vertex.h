#pragma once
#include "glm/glm.hpp"


/// @brief used to store vertex data for meshes
struct Vertex
{
    /// @brief position data for vertex
    glm::vec3 position;
    /// @brief normal of vertex
    glm::vec3 normal;
    /// @brief the coords of the texture corresponding to this vertex
    glm::vec2 texture_coords;
};