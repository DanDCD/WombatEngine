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
    Model(const char *path);

    void draw(Shader &shader);

private:
    void loadModel(std::string path);

    void processNode(aiNode *node, const aiScene *scene);

    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    std::vector<TextureInfo> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

    std::vector<Mesh> meshes;
    std::string directory;
};