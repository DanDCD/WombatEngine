#include "root/graphics/implementation/mesh.h"
#include "rendering/vertex/vertex.h"
#include <string>
#include "rendering/log/check_gl.h"
#include "utils/logging/logging.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, entt::resource<Material> material)
    : vertices(vertices), indices(indices), material(material), vao()
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

Mesh::Mesh(Mesh &&other)
    : vao(std::move(other.vao))
{
    this->vertices = other.vertices;
    this->indices = other.indices;
    this->textures = other.textures;
    this->shininess = other.shininess;
    this->material = other.material;
    other.vertices.clear();
    other.indices.clear();
    other.textures.clear();
    other.shininess = 0.0f;
}

Mesh &Mesh::operator=(Mesh &&other) noexcept
{
    this->vertices = other.vertices;
    this->indices = other.indices;
    this->textures = other.textures;
    this->shininess = other.shininess;
    this->material = other.material;
    this->vao = std::move(other.vao);
    other.vertices.clear();
    other.indices.clear();
    other.textures.clear();
    other.shininess = 0.0f;
    return *this;
}

Mesh::~Mesh()
{
}

void Mesh::draw(entt::resource<Shader> shader)
{
    shader->use();
    vao.bind();

    // bind all textures
    auto textures = material->getTexturesFlat();
    unsigned int num_diffuse = material->diffuse_maps.size();
    unsigned int num_specular = material->specular_maps.size();

    for (int tex_unit = 0; tex_unit < num_diffuse; tex_unit++) // for each diffuse texture bind it to the corresponding uniform in the frag shader's material
    {
        std::string uniform_name = "material.texture_diffuse" + std::to_string(tex_unit);
        shader->setUniform(uniform_name, tex_unit);
        textures[tex_unit]->bind(GL_TEXTURE0 + tex_unit);
        checkGLError("POST BINDING " + uniform_name);
    }
    for (int tex_unit = num_diffuse; tex_unit < num_diffuse + num_specular; tex_unit++)
    {
        std::string uniform_name = "material.texture_specular" + std::to_string(tex_unit-num_diffuse);
        shader->setUniform(uniform_name, tex_unit);
        textures[tex_unit]->bind(GL_TEXTURE0+ tex_unit);
        checkGLError("POST BINDING " + uniform_name);
    }
    shader->setUniform("material.shininess", material->shininess);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    // // bind textures associated with this mesh to their respective uniforms
    // unsigned int num_diffuse = 0, num_specular = 0, num_other = 0; // the current number of diffuse/specular shaders processed by this mesh
    // for (auto &textureInfo : textures)
    // {
    //     // convert weak texture pointer into shared pointer
    //     if (auto texture_ptr = textureInfo.texture.lock())
    //     {
    //         std::string uniformName = "material.texture_";
    //         if (texture_ptr->getUseCase() == Texture::TEXTURE_USECASE::DIFFUSE)
    //             uniformName.append("diffuse" + std::to_string(num_diffuse++)); // e.g. material.texture_diffuse0, ...
    //         else if (texture_ptr->getUseCase() == Texture::TEXTURE_USECASE::SPECULAR)
    //             uniformName.append("specular" + std::to_string(num_specular++));
    //         else if (texture_ptr->getUseCase() == Texture::TEXTURE_USECASE::OTHER)
    //             uniformName.append("other" + std::to_string(num_other++));
    //         else
    //             LOG("Invalid texture usecase in draw call for: " + textureInfo.file_path, Logging::LOG_TYPE::ERROR);
    //         shader.setUniform(uniformName, (int)texture_ptr->getTextureUnit());
    //         texture_ptr->bind();
    //     }
    //     else // if the weak ptr in texture info has expired, log an error and skip it
    //         LOG("Trying to bind non-existent texture: " + textureInfo.file_path, Logging::LOG_TYPE::ERROR);
    // }
    // // bind the 'shininess' of this mesh to its uniform
    // shader.setUniform("material.shininess", shininess);
}