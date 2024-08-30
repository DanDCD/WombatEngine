#include "rendering/assimp/mesh.h"
#include "rendering/vertex/vertex.h"
#include <string>
#include "rendering/log/check_gl.h"
#include "utils/logging/logging.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureInfo> textures, float shininess)
    : vao()
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->shininess = shininess;
    setupMesh();
}

void Mesh::setupMesh()
{
    VBO vbo = VBO(GL_ARRAY_BUFFER);
    vbo.assignData(&vertices[0], vertices.size() * sizeof(Vertex), GL_STATIC_DRAW);

    EBO ebo = EBO();
    ebo.assignData(&indices[0], indices.size() * sizeof(unsigned int), GL_STATIC_DRAW);

    VertexBufferLayout layout = VertexBufferLayout();
    layout.addAttribute(GL_FLOAT, 3, 3 * sizeof(float), GL_FALSE); // verts
    layout.addAttribute(GL_FLOAT, 3, 3 * sizeof(float), GL_FALSE); // normals
    layout.addAttribute(GL_FLOAT, 2, 2 * sizeof(float), GL_FALSE); // texture coords

    vao.addBuffer(std::move(vbo), layout);
    vao.addBuffer(std::move(ebo));
}

void Mesh::draw(Shader &shader)
{
    shader.use();
    vao.bind();

    // bind textures associated with this mesh to their respective uniforms
    unsigned int num_diffuse = 0, num_specular = 0, num_other = 0; // the current number of diffuse/specular shaders processed by this mesh
    for (auto &textureInfo : textures)
    {
        // convert weak texture pointer into shared pointer
        if (auto texture_ptr = textureInfo.texture.lock())
        {
            std::string uniformName = "material.texture_";
            if (texture_ptr->getUseCase() == Texture::TEXTURE_USECASE::DIFFUSE)
                uniformName.append("diffuse" + std::to_string(num_diffuse++)); // e.g. material.texture_diffuse0, ...
            else if (texture_ptr->getUseCase() == Texture::TEXTURE_USECASE::SPECULAR)
                uniformName.append("specular" + std::to_string(num_specular++));
            else if (texture_ptr->getUseCase() == Texture::TEXTURE_USECASE::OTHER)
                uniformName.append("other" + std::to_string(num_other++));
            else
                LOG("Invalid texture usecase in draw call for: " + textureInfo.file_path, Logging::LOG_TYPE::ERROR);
            shader.setUniform(uniformName, (int)texture_ptr->getTextureUnit());
            texture_ptr->bind();
        }
        else // if the weak ptr in texture info has expired, log an error and skip it
            LOG("Trying to bind non-existent texture: " + textureInfo.file_path, Logging::LOG_TYPE::ERROR);
    }
    // bind the 'shininess' of this mesh to its uniform
    shader.setUniform("material.shininess", shininess);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}