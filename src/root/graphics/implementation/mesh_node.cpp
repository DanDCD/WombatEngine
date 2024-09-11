#include "root/graphics/implementation/mesh_node.h"

MeshNode::MeshNode()
    : meshes(), child_nodes()
{
}

MeshNode::MeshNode(std::vector<entt::resource<Mesh>> meshes, std::vector<entt::resource<MeshNode>> child_nodes, glm::mat4 offset)
    : meshes(meshes), child_nodes(child_nodes), offset(offset)
{
}

MeshNode::~MeshNode()
{
}

MeshNode::MeshNode(MeshNode &&other)
    : meshes(std::move(other.meshes)), child_nodes(std::move(other.child_nodes))
{
}

MeshNode &MeshNode::operator=(MeshNode &&other) noexcept
{
    this->meshes = std::move(other.meshes);
    this->child_nodes = std::move(other.child_nodes);
    return *this;
}

void MeshNode::draw(entt::resource<Shader> &shader)
{
    for (auto mesh : meshes)
        mesh->draw(*shader);

    for (auto mesh_node : child_nodes)
        mesh_node->draw(shader);
}