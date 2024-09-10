#include "root/graphics/interface/resource_manager.h"
#include "utils/logging/logging.h"
#include "glm/glm.hpp"

template <typename T>
template <typename... Args>
typename Wombat::Graphics::Loader<T>::result_type Wombat::Graphics::Loader<T>::operator()(Args &&...args) const
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

Wombat::Graphics::ResourceManager::ResourceManager()
    : shader_cache(), texture_cache(), material_cache(), mesh_cache(), mesh_node_cache()
{
    LOG("ResourceManager initialised", Logging::LOG_TYPE::INFO, Logging::LOG_PRIORITY::MEDIUM);
}

Wombat::Graphics::ResourceManager::~ResourceManager()
{
    shader_cache.clear();
    texture_cache.clear();
    material_cache.clear();
    mesh_cache.clear();
    mesh_node_cache.clear();
    LOG("ResourceManager destroyed", Logging::LOG_TYPE::INFO, Logging::LOG_PRIORITY::MEDIUM);
}

Wombat::Graphics::ResourceManager::ResourceManager(ResourceManager &&other)
    : shader_cache(std::move(other.shader_cache)), texture_cache(std::move(other.texture_cache)),
      material_cache(std::move(other.material_cache)), mesh_cache(std::move(other.mesh_cache)),
      mesh_node_cache(std::move(other.mesh_node_cache))
{
}

Wombat::Graphics::ResourceManager &Wombat::Graphics::ResourceManager::operator=(ResourceManager &&other) noexcept
{
    this->shader_cache = std::move(other.shader_cache);
    this->texture_cache = std::move(other.texture_cache);
    this->material_cache = std::move(other.material_cache);
    this->mesh_cache = std::move(other.mesh_cache);
    this->mesh_node_cache = std::move(other.mesh_node_cache);
    return *this;
}

entt::resource<Mesh> Wombat::Graphics::ResourceManager::load_model(const std::string &model_path)
{
    // create the id for the root or 'top' mesh
    std::string root_mesh_path = "loaded_mesh/" + model_path;
    entt::id_type root_mesh_id = entt::hashed_string::value(root_mesh_path.c_str());

    LOG("Attempting to load model from " + model_path, Logging::LOG_TYPE::INFO, Logging::LOG_PRIORITY::MEDIUM);
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(model_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

    // if loading failed
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        LOG(std::string("Error loading model with ASSIMP ") + importer.GetErrorString(), Logging::LOG_TYPE::ERROR);
    }
    LOG("Assimp successfuly read model file " + model_path, Logging::LOG_TYPE::INFO, Logging::LOG_PRIORITY::MEDIUM);

    aiNode *root = scene->mRootNode;

    return mesh_cache[root_mesh_id];
}

entt::resource<MeshNode> Wombat::Graphics::ResourceManager::process_ai_mesh_node(std::string mesh_node_id_str, aiNode *mesh_node, aiScene *scene)
{
    // data used to construct new mesh node
    std::vector<entt::resource<Mesh>> meshes;          // the meshes belonging to this node
    std::vector<entt::resource<MeshNode>> child_nodes; // the mesh nodes belonging to this node
    glm::mat4 offset;                                  // the offset this node has from its parent

    for (unsigned int i = 0; i < mesh_node->mNumMeshes; i++)
    {
        // create id for mesh
        std::string mesh_id_str;
        // TODO: ...

        // obtain aiMesh
        unsigned int mesh_index = mesh_node->mMeshes[i];
        aiMesh *mesh = scene->mMeshes[mesh_index];
        // process mesh
        entt::resource<Mesh> mesh_res = process_ai_mesh(mesh_id_str, mesh, scene);

        meshes.push_back(mesh_res);
    }

    for (unsigned int i = 0; i < mesh_node->mNumChildren; i++)
    {
        // create id for child
        std::string child_id_str;
        // TODO: ...

        // obtain aiNode child
        aiNode *ai_child_node = mesh_node->mChildren[i];
        // process child
        entt::resource<MeshNode> child_node = process_ai_mesh_node(child_id_str, ai_child_node, scene);

        child_nodes.push_back(child_node);
    }

    // convert ai matrix into glm matrix
    aiMatrix4x4 ai_offset = mesh_node->mTransformation;
    offset = glm::mat4(ai_offset.a1, ai_offset.a2, ai_offset.a3, ai_offset.a4,
                       ai_offset.b1, ai_offset.b2, ai_offset.b3, ai_offset.b4,
                       ai_offset.c1, ai_offset.c2, ai_offset.c3, ai_offset.c4,
                       ai_offset.d1, ai_offset.d2, ai_offset.d3, ai_offset.d4);

    // finally create the mesh node itself in the mesh node cache
    auto mesh_node_id = entt::hashed_string::value(mesh_node_id_str.c_str());
    mesh_node_cache.load(mesh_node_id, meshes, child_nodes, offset);
    return mesh_node_cache[mesh_node_id];
}

entt::resource<Mesh> Wombat::Graphics::ResourceManager::process_ai_mesh(std::string mesh_id_str, const aiMesh *mesh, const aiScene *scene)
{
}