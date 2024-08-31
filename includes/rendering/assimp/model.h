#pragma once
#include <vector>
#include "rendering/shader/shader.h"
#include "rendering/assimp/mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "rendering/texture/texture.h"
#include "rendering/texture/texture_manager.h"

class Model
{
public:
    /// @brief
    /// @param path
    Model(const char *path);

    /// @brief
    /// @param other
    Model(Model &&other);

    /// @brief
    /// @param other
    /// @return
    Model &operator=(Model &&other) noexcept;

    /// @brief delete the copy constructor
    /// @param
    Model(const Model &) = delete;

    /// @brief delete the copy assignment operator
    /// @param
    /// @return
    Model &operator=(const Model &) = delete;

    /// @brief destructor
    ~Model();

    /// @brief
    /// @param shader
    void draw(Shader &shader);

private:
    /// @brief
    /// @param path
    void loadModel(std::string path);

    /// @brief
    /// @param node
    /// @param scene
    void processNode(aiNode *node, const aiScene *scene);

    /// @brief
    /// @param mesh
    /// @param scene
    /// @return
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    /// @brief
    /// @param mat
    /// @param type
    /// @param typeName
    /// @param count_offset
    /// @return
    std::vector<TextureInfo> loadMaterialTextures(aiMaterial *mat, aiTextureType type, unsigned int count_offset);

    /// @brief
    std::vector<Mesh> meshes;

    /// @brief
    std::string directory;
};