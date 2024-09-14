#pragma once
#include "root/graphics/implementation/mesh.h"
#include "dependencies/entt/entt.hpp"
#include "glm/glm.hpp"
#include <memory>

class MeshNode
{
public:
    MeshNode();
    MeshNode(std::vector<entt::resource<Mesh>> meshes, std::vector<entt::resource<MeshNode>> child_nodes, glm::mat4 offset);
    ~MeshNode();

    MeshNode(MeshNode &&other);
    MeshNode &operator=(MeshNode &&other) noexcept;

    MeshNode(const MeshNode &) = delete;
    MeshNode &operator=(const MeshNode &) = delete;

    void draw(entt::resource<Shader> shader);


    /// @brief meshes belonging to this mesh node
    std::vector<entt::resource<Mesh>> meshes;
    /// @brief child mesh nodes that belong to this node - may be offset
    std::vector<entt::resource<MeshNode>> child_nodes;
    /// @brief the offset of this node from its parent
    glm::mat4 offset;
private:
};