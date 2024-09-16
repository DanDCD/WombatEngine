#include "root/graphics/implementation/mesh.h"
#include <string>
#include "root/graphics/implementation/check_gl.h"
#include "root/utils/logging.h"

Mesh::Mesh(std::vector<Wombat::Graphics::Vertex> vertices, std::vector<unsigned int> indices, entt::resource<Material> material)
    : vertices(vertices), indices(indices), material(material), vao()
{
    Wombat::Graphics::VBO vbo = Wombat::Graphics::VBO(GL_ARRAY_BUFFER);
    vbo.assignData(&vertices[0], vertices.size() * sizeof(Wombat::Graphics::Vertex), GL_STATIC_DRAW);

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
    this->material = other.material;
    other.vertices.clear();
    other.indices.clear();
}

Mesh &Mesh::operator=(Mesh &&other) noexcept
{
    this->vertices = other.vertices;
    this->indices = other.indices;
    this->material = other.material;
    this->vao = std::move(other.vao);
    other.vertices.clear();
    other.indices.clear();
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
        textures[tex_unit]->bind(tex_unit);
        checkGLError("POST BINDING " + uniform_name);
    }
    for (int tex_unit = num_diffuse; tex_unit < num_diffuse + num_specular; tex_unit++)
    {
        std::string uniform_name = "material.texture_specular" + std::to_string(tex_unit - num_diffuse);
        shader->setUniform(uniform_name, tex_unit);
        textures[tex_unit]->bind(tex_unit);
        checkGLError("POST BINDING " + uniform_name);
    }
    shader->setUniform("material.shininess", material->shininess);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}