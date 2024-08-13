#include "rendering/assimp/mesh.h"
#include "rendering/vertex/vertex.h"
#include <string>
#include "rendering/log/check_gl.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureInfo> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

void Mesh::setupMesh()
{
    VAO vao = VAO();

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

    this->vao = std::move(vao);
}

void Mesh::draw(Shader &shader)
{
    vao.bind();

    unsigned int num_diffuse, num_specular; // the current number of diffuse/specular shaders processed by this mesh
    for(auto &textureInfo : textures)
    {
        int x;
    }

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}