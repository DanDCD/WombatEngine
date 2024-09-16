#include "root/graphics/interface/resource_manager.h"
#include "root/utils/logging.h"
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

entt::resource<MeshNode> Wombat::Graphics::ResourceManager::load_model(const std::string &model_path)
{
    // create the id for the root or 'top' mesh
    std::string dir = model_path.substr(0, model_path.find_last_of('/')); // assign the directory the path ends at (not the file)
    std::string root_mesh_id_str = "loaded_mesh/" + dir;                  // the string representation of the hash id

    // check that this model has not already been loaded
    auto root_mesh_id = entt::hashed_string::value(root_mesh_id_str.c_str());
    if (mesh_node_cache.contains(root_mesh_id))
        return mesh_node_cache[root_mesh_id];

    // load assimp model
    LOG("Attempting to load model from " + model_path, Logging::LOG_TYPE::INFO, Logging::LOG_PRIORITY::MEDIUM);
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(model_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
    // if loading failed
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        LOG(std::string("Error loading model with ASSIMP ") + importer.GetErrorString(), Logging::LOG_TYPE::ERROR);
    }
    LOG("Assimp successfuly read model file " + model_path, Logging::LOG_TYPE::INFO, Logging::LOG_PRIORITY::MEDIUM);
    aiNode *root_node = scene->mRootNode;
    return process_ai_mesh_node(root_mesh_id_str, dir, root_node, scene);
}

entt::resource<Shader> Wombat::Graphics::ResourceManager::load_shader(const std::string &vertex_shader_path, const std::string &fragment_shader_path)
{
    std::string shader_id_str = "shaders/" + vertex_shader_path + fragment_shader_path;
    auto shader_id = entt::hashed_string::value(shader_id_str.c_str());

    // check the shader hasnt already been loaded
    if(shader_cache.contains(shader_id))
        return shader_cache[shader_id];

    // otherwise load shader
    shader_cache.load(shader_id, vertex_shader_path.c_str(), fragment_shader_path.c_str());
    return shader_cache[shader_id];
}

entt::resource<MeshNode> Wombat::Graphics::ResourceManager::process_ai_mesh_node(std::string mesh_node_id_str, const std::string dir, const aiNode *mesh_node, const aiScene *scene)
{
    // data used to construct new mesh node
    std::vector<entt::resource<Mesh>> meshes;          // the meshes belonging to this node
    std::vector<entt::resource<MeshNode>> child_nodes; // the mesh nodes belonging to this node
    glm::mat4 offset;                                  // the offset this node has from its parent

    for (unsigned int i = 0; i < mesh_node->mNumMeshes; i++)
    {
        // create id for mesh - we need to update this string to convey this is a mesh of this mesh node
        std::string mesh_id_str = mesh_node_id_str + "/meshes/" + std::to_string(i);
        // obtain aiMesh
        unsigned int mesh_index = mesh_node->mMeshes[i];
        aiMesh *mesh = scene->mMeshes[mesh_index];
        // process mesh
        entt::resource<Mesh> mesh_res = process_ai_mesh(mesh_id_str, dir, mesh, scene);
        meshes.push_back(mesh_res);
    }
    for (unsigned int i = 0; i < mesh_node->mNumChildren; i++)
    {
        // create id for child - we need to update this string to convey this is a child node of this mesh node
        std::string child_id_str = mesh_node_id_str + "/children/" + std::to_string(i);
        // obtain aiNode child
        aiNode *ai_child_node = mesh_node->mChildren[i];
        // process child recursively
        entt::resource<MeshNode> child_node = process_ai_mesh_node(child_id_str, dir, ai_child_node, scene);
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

entt::resource<Mesh> Wombat::Graphics::ResourceManager::process_ai_mesh(std::string mesh_id_str, const std::string dir, const aiMesh *mesh, const aiScene *scene)
{
    std::vector<Wombat::Graphics::Vertex> vertices;      // the vertices belonging to this mesh
    std::vector<unsigned int> indices; // the indices belonging to this mesh
    entt::resource<Material> material; // the material of this mesh

    // populate vertices from the vertices of the mesh
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Wombat::Graphics::Vertex vertex;
        glm::vec3 position;
        position.x = mesh->mVertices[i].x;
        position.y = mesh->mVertices[i].y;
        position.z = mesh->mVertices[i].z;
        vertex.position = position;
        if (mesh->HasNormals())
        {
            glm::vec3 normal;
            normal.x = mesh->mNormals[i].x;
            normal.y = mesh->mNormals[i].y;
            normal.z = mesh->mNormals[i].z;
            vertex.normal = normal;
        }
        if (mesh->mTextureCoords[0])
        {
            glm::vec2 texcoord;
            texcoord.x = mesh->mTextureCoords[0][i].x;
            texcoord.y = mesh->mTextureCoords[0][i].y;
            vertex.texture_coords = texcoord;
        }
        else
            vertex.texture_coords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // populate indices from the faces of the mesh
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    if (mesh->mMaterialIndex >= 0)                                                                            // if this mesh is associated with a material, load/fetch it
        material = process_ai_material("loaded_material/" + dir + "/" + std::to_string(mesh->mMaterialIndex), // note: materials belong to scenes not meshes in assimp
                                       dir,
                                       scene->mMaterials[mesh->mMaterialIndex],
                                       scene);
    else // otherwise use the default material (TODO: define a default material in resource manager constructor)
    {
    }
    auto mesh_id = entt::hashed_string::value(mesh_id_str.c_str());
    mesh_cache.load(mesh_id, vertices, indices, material);
    return mesh_cache[mesh_id];
}

entt::resource<Material> Wombat::Graphics::ResourceManager::process_ai_material(std::string material_id_str, const std::string dir, const aiMaterial *material, const aiScene *scene)
{
    // if we have already loaded this material, return it
    auto material_id = entt::hashed_string::value(material_id_str.c_str());
    if (material_cache.contains(material_id))
        return material_cache[material_id];
    // load diffuse maps
    std::vector<entt::resource<Texture>> diffuse_maps;
    unsigned int num_diffuse = material->GetTextureCount(aiTextureType_DIFFUSE);
    for (unsigned int i = 0; i < num_diffuse; i++)
    {
        aiString texture_name;
        material->GetTexture(aiTextureType_DIFFUSE, i, &texture_name);
        std::string path;
        path += dir.c_str();
        path += "/";
        path += texture_name.C_Str();
        diffuse_maps.push_back(process_texture("loaded_texture/" + path, path));
    }
    // load specular maps
    std::vector<entt::resource<Texture>> specular_maps;
    unsigned int num_specular = material->GetTextureCount(aiTextureType_SPECULAR);
    for (unsigned int i = 0; i < num_specular; i++)
    {
        aiString texture_name;
        material->GetTexture(aiTextureType_SPECULAR, i, &texture_name);
        std::string path;
        path += dir.c_str();
        path += "/";
        path += texture_name.C_Str();
        specular_maps.push_back(process_texture("loaded_texture/" + path, path));
    }

    // loaded shininess
    float shininess;
    bool shininess_loaded = aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess);
    if (!shininess_loaded)
    {
        LOG("Failed to load shininess value from Assimp Material", Logging::LOG_TYPE::WARNING, Logging::LOG_PRIORITY::LOW);
        shininess = 64.0f;
    }

    material_cache.load(material_id, shininess, diffuse_maps, specular_maps);
    return material_cache[material_id];
}

entt::resource<Texture> Wombat::Graphics::ResourceManager::process_texture(std::string texture_id_str, const std::string path)
{
    // check if texture is already loaded
    auto texture_id = entt::hashed_string::value(texture_id_str.c_str());
    if (texture_cache.contains(texture_id))
        return texture_cache[texture_id];

    texture_cache.load(texture_id,
                       GL_TEXTURE_2D,
                       std::vector<TextureParam>{
                           TextureParam(GL_TEXTURE_WRAP_S, GL_REPEAT),
                           TextureParam(GL_TEXTURE_WRAP_T, GL_REPEAT),
                           TextureParam(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR),
                           TextureParam(GL_TEXTURE_MAG_FILTER, GL_LINEAR)},
                       path);
    return texture_cache[texture_id];
}