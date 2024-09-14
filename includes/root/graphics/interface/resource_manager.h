#pragma once
#include "dependencies/entt/entt.hpp"
#include "root/graphics/implementation/shader.h"
#include "root/graphics/implementation/texture.h"
#include "root/graphics/implementation/material.h"
#include "root/graphics/implementation/shader.h"
#include "root/graphics/implementation/mesh.h"
#include "root/graphics/implementation/mesh_node.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Wombat
{
    namespace Graphics
    {
        /// @brief A loader struct to be given to an entt cache and used to load resources of type T
        /// @tparam T the type of the resource to be loaded
        template <typename T>
        struct Loader final
        {
            /// @brief the type we expect this loader to return - required for entt cache to work
            using result_type = std::shared_ptr<T>;

            /// @brief callable operator used by entt to instantiate the type
            /// @tparam ...Args used to allow a variable number of arguments
            /// @param ...args arguments used to construct T
            /// @return a shared pointer to T
            template <typename... Args>
            result_type operator()(Args &&...args) const;
        };

        // aliases for loaders
        using ShaderLoader = Loader<Shader>;
        using TextureLoader = Loader<Texture>;
        using MaterialLoader = Loader<Material>;
        using MeshLoader = Loader<Mesh>;
        using MeshNodeLoader = Loader<MeshNode>;

        // aliases for caches
        using ShaderCache = entt::resource_cache<Shader, ShaderLoader>;
        using TextureCache = entt::resource_cache<Texture, TextureLoader>;
        using MaterialCache = entt::resource_cache<Material, MaterialLoader>;
        using MeshCache = entt::resource_cache<Mesh, MeshLoader>;
        using MeshNodeCache = entt::resource_cache<MeshNode, MeshNodeLoader>;

        /// @brief A class responsible for owning resource caches and organising resource loading
        class ResourceManager
        {
        public:
            /// @brief default constructor will initialise empty caches
            ResourceManager();

            /// @brief destructor will destroy caches and their loaded resources (will not necessarily unload all data due to strong pointers)
            ~ResourceManager();

            /// @brief
            /// @param other
            ResourceManager(ResourceManager &&other);

            /// @brief
            /// @param other
            /// @return
            ResourceManager &operator=(ResourceManager &&other) noexcept;

            /// @brief delete the copy constructor
            /// @param
            ResourceManager(const ResourceManager &) = delete;

            /// @brief delete the copy assignment operator
            /// @param
            /// @return
            ResourceManager &operator=(const ResourceManager &) = delete;

            /// @brief loads a model from the given path, loading each submesh and returning the top mesh
            /// @param model_path the path of the model file
            /// @return the top mesh of the model hierarchy
            entt::resource<MeshNode> load_model(const std::string &model_path);

            /// @brief loads a shader program from vertex and fragment shader files at the given paths
            /// @param vertex_shader_path 
            /// @param fragment_shader_path 
            /// @return a handle to the loaded shader
            entt::resource<Shader> load_shader(const std::string &vertex_shader_path, const std::string &fragment_shader_path);


            ShaderCache shader_cache;
            TextureCache texture_cache;
            MaterialCache material_cache;
            MeshCache mesh_cache;
            MeshNodeCache mesh_node_cache;

        private:
            /// @brief process an ai mesh node by registering it with a mesh cache after processing its child nodes
            /// @param mesh_node_id_str the string form of the id to be associated with this node in the mesh node cache
            /// @param dir the directory that this assimp model was loaded from (this will remain the same for the whole mesh_node tree)
            /// @param mesh_node the assimp mesh node to process
            /// @param scene the assimp scene associated with this mesh node
            /// @return the handle for this meshnode
            entt::resource<MeshNode> process_ai_mesh_node(std::string mesh_node_id_str, const std::string dir, const aiNode *mesh_node, const aiScene *scene);

            /// @brief
            /// @param mesh_id_str
            /// @param dir
            /// @param mesh
            /// @param scene
            /// @return
            entt::resource<Mesh> process_ai_mesh(std::string mesh_id_str, const std::string dir, const aiMesh *mesh, const aiScene *scene);

            /// @brief
            /// @param material_id_str
            /// @param material
            /// @param scene
            /// @return
            entt::resource<Material> process_ai_material(std::string material_id_str, const std::string dir, const aiMaterial *material, const aiScene *scene);

            /// @brief 
            /// @param texture_id_str 
            /// @param path 
            /// @param usecase 
            /// @param texture_buffer_index 
            /// @return 
            entt::resource<Texture> process_texture(std::string texture_id_str, const std::string path);
        };

    }
}