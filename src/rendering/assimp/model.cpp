#include "rendering/assimp/model.h"
#include "iostream"

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
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    // if loading failed
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

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
    std::vector<Mesh::Texture> textures;

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
        // if (mesh->HasNormals())
        // {
        //     vector.x = mesh->mNormals[i].x;
        //     vector.y = mesh->mNormals[i].y;
        //     vector.z = mesh->mNormals[i].z;
        //     vertex.normal = vector;
        // }
        // texture coords
        // if (mesh->mTextureCoords[0])
        // {
        //     glm::vec2 vec;
        //     // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
        //     // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
        //     vec.x = mesh->mTextureCoords[0][i].x;
        //     vec.y = mesh->mTextureCoords[0][i].y;
        //     vertex.texture_coords = vec;
        //     // tangent
        //     // vector.x = mesh->mTangents[i].x;
        //     // vector.y = mesh->mTangents[i].y;
        //     // vector.z = mesh->mTangents[i].z;
        //     // vertex.Tangent = vector;
        //     // bitangent
        //     // vector.x = mesh->mBitangents[i].x;
        //     // vector.y = mesh->mBitangents[i].y;
        //     // vector.z = mesh->mBitangents[i].z;
        //     // vertex.Bitangent = vector;
        // }
        // else
        //     vertex.texture_coords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // populate indices from the faces of the mesh
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // populate textures from the material of the mesh
    if (mesh->mMaterialIndex >= 0)
    {
        // aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        // std::vector<Mesh::Texture> diffuseMaps = loadMaterialTextures(material,
        //                                                               aiTextureType_DIFFUSE, "texture_diffuse");
        // textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // std::vector<Mesh::Texture> specularMaps = loadMaterialTextures(material,
        //                                                                aiTextureType_SPECULAR, "texture_specular");
        // textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    return Mesh(vertices, indices, textures);
}

std::vector<Mesh::Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    return {};
}

