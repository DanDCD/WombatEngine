#include "rendering/assimp/model.h"
#include "iostream"
#include "utils/logging/logging.h"
#include "string"

Model::Model(const char *path)
{
    loadModel(path);
}

void Model::draw(Shader &shader)
{
    for (auto &mesh : meshes)
        mesh.draw(shader);
}

void Model::loadModel(std::string path)
{
    // when we import the model, if it contains non triangular primitives, make them triangular
    // where necessary, flip the texture coords
    LOG("Attempting to load model from " + path, Logging::LOG_TYPE::INFO, Logging::LOG_PRIORITY::MEDIUM);
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

    // if loading failed
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        LOG(std::string("Error loading model with ASSIMP ") + importer.GetErrorString(), Logging::LOG_TYPE::ERROR);
        return;
    }
    LOG("Assimp successfuly read model file " + path, Logging::LOG_TYPE::INFO, Logging::LOG_PRIORITY::MEDIUM);

    directory = path.substr(0, path.find_last_of('/')); // assign the directory the path ends at (not the file)
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{

    // process all the meshes belonging to this node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        unsigned int mesh_index = node->mMeshes[i];
        aiMesh *mesh = scene->mMeshes[mesh_index]; // recall that the scene contains the actual mesh data

        meshes.push_back(processMesh(mesh, scene));
    }

    // then process all the child nodes belonging to the node
    for (unsigned int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene);
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<TextureInfo> textures;

    // populate vertices from the vertices of the mesh
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // placeholder vector
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }
        // texture coords
        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texture_coords = vec;
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
    float shininess;
    // populate textures from the material of the mesh
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex]; // get material from assimp scene

        // load the different texture types and add them to the textures list (note: we have to use move iterators as Texture has deleted copying)
        std::vector<TextureInfo> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, 0);
        textures.insert(textures.end(), std::make_move_iterator(diffuseMaps.begin()), std::make_move_iterator(diffuseMaps.end()));

        std::vector<TextureInfo> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, textures.size());
        textures.insert(textures.end(), std::make_move_iterator(specularMaps.begin()), std::make_move_iterator(specularMaps.end()));

        // load shininess from assimp material

        bool shininess_loaded = aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess);
        if (!shininess_loaded)
        {
            LOG("Failed to load shininess value from Assimp Material", Logging::LOG_TYPE::WARNING, Logging::LOG_PRIORITY::LOW);
            shininess = 64.0f;
        }
    }
    return Mesh(vertices, indices, textures, shininess);
}

std::vector<TextureInfo> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, unsigned int count_offset)
{
    std::vector<TextureInfo> textures;
    unsigned int numTexturesInMat = mat->GetTextureCount(type);

    Texture::TEXTURE_USECASE usecase;
    switch (type)
    {
    case aiTextureType_DIFFUSE:
        usecase = Texture::TEXTURE_USECASE::DIFFUSE;
        break;
    case aiTextureType_SPECULAR:
        usecase = Texture::TEXTURE_USECASE::SPECULAR;
        break;
    default:
        usecase = Texture::TEXTURE_USECASE::OTHER;
        break;
    }

    for (unsigned int i = 0; i < numTexturesInMat; i++)
    {
        // get the location of texture of type 'type' and index 'i'
        aiString textureName;
        mat->GetTexture(type, i, &textureName);

        std::string fullPath;
        fullPath += directory.c_str();
        fullPath += "/";
        fullPath += textureName.C_Str();

        auto texture_info = TextureManager::loadNewTexture(fullPath, usecase, count_offset + i);
        textures.push_back(texture_info);
    }
    return textures;
}
