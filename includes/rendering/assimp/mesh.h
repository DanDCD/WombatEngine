#pragma once
#include "glm/glm.hpp"
#include <string>
#include <vector>
#include "rendering/shader/shader.h"
#include "rendering/VAO/vao.h"
#include "rendering/buffer/vbo/vbo.h"
#include "rendering/buffer/ebo/ebo.h"
#include "rendering/texture/texture.h"
#include "rendering/texture/texture_manager.h"

class Mesh
{
public:
    /// @brief
    /// @param vertices
    /// @param indices
    /// @param textures
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureInfo> textures);

    /// @brief
    /// @param shader
    void draw(Shader &shader);

private:
    void setupMesh();

    /// @brief
    std::vector<Vertex> vertices;

    /// @brief
    std::vector<unsigned int> indices;

    /// @brief
    std::vector<TextureInfo> textures;

    VAO vao;
};