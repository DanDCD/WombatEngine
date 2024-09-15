#pragma once
#include "glm/glm.hpp"
#include <string>
#include <vector>
#include "root/graphics/implementation/shader.h"
#include "root/graphics/implementation/vao.h"
#include "rendering/buffer/vbo/vbo.h"
#include "rendering/buffer/ebo/ebo.h"
#include "root/graphics/implementation/texture.h"
#include "dependencies/entt/entt.hpp"
#include "root/graphics/implementation/material.h"

class Mesh
{
public:
    /// @brief 
    /// @param vertices 
    /// @param indices 
    /// @param material 
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, entt::resource<Material> material);


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
    void draw(entt::resource<Shader> shader);

private:
    /// @brief the vertices associated with this mesh
    std::vector<Vertex> vertices;

    /// @brief the indices defining the order in which vertices are drawn
    std::vector<unsigned int> indices;

    /// @brief the material associated with this mesh
    entt::resource<Material> material;

    /// @brief this mesh's VAO (containing the VBO and EBO)
    VAO vao;
};