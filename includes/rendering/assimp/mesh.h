#pragma once
#include "glm/glm.hpp"
#include <string>
#include <vector>
#include "rendering/shader/shader.h"
#include "rendering/VAO/vao.h"
#include "rendering/buffer/vbo/vbo.h"
#include "rendering/buffer/ebo/ebo.h"

class Mesh
{
public:
    /// @brief used to store texture data for meshes
    struct Texture
    {
        /// @brief the id of this texture in the Assimp scene's materials
        unsigned int id;
        /// @brief the type of this texture (e.g. a diffuse or specular)
        std::string type;

        std::string path;
    };

    /// @brief
    /// @param vertices
    /// @param indices
    /// @param textures
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Mesh::Texture> textures);

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
    std::vector<Mesh::Texture> textures;

    VAO vao;
};