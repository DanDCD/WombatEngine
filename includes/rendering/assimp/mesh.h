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
    /// @brief constructor
    /// @param vertices the vertex data (verts, normals, texture coords)
    /// @param indices the indices for the vertex data
    /// @param textures list of textures associated with this mesh (specular, diffuse, etc.)
    /// @param shininess the shininiess value of this mesh's material
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureInfo> textures, float shininess);

    /// @brief
    /// @param other
    Mesh(Mesh &&other);

    /// @brief
    /// @param other
    /// @return
    Mesh &operator=(Mesh &&other) noexcept;

    /// @brief delete the copy constructor
    /// @param
    Mesh(const Mesh &) = delete;

    /// @brief delete the copy assignment operator
    /// @param
    /// @return
    Mesh &operator=(const Mesh &) = delete;

    ~Mesh();

    /// @brief draw this mesh
    /// @param shader the shader to render this mesh with
    void draw(Shader &shader);

private:
    /// @brief create VAO, VBO, and EBO for this mesh in OpenGL
    void setupMesh();

    /// @brief the vertices associated with this mesh
    std::vector<Vertex> vertices;

    /// @brief the indices defining the order in which vertices are drawn
    std::vector<unsigned int> indices;

    /// @brief the textures associated with this mesh
    std::vector<TextureInfo> textures;

    /// @brief the shininess of this mesh's material
    float shininess;

    /// @brief this mesh's VAO (containing the VBO and EBO)
    VAO vao;
};