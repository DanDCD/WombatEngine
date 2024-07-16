#include "rendering/assimp/mesh.h"
#include "rendering/vertex/vertex.h"
#include <string>
#include "rendering/log/check_gl.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Mesh::Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

void Mesh::setupMesh()
{
    // TODO: use classes
    // vao = VAO();

    // VBO vbo = VBO(GL_ARRAY_BUFFER);
    // vbo.assignData(&vertices[0], vertices.size() * sizeof(Vertex), GL_STATIC_DRAW);

    // EBO ebo = EBO();
    // ebo.assignData(&indices[0], indices.size() * sizeof(unsigned int), GL_STATIC_DRAW);

    // VertexBufferLayout layout = VertexBufferLayout();
    // layout.addAttribute(GL_FLOAT, 3, 3 * sizeof(float), GL_FALSE); // verts
    // // layout.addAttribute(GL_FLOAT, 3, 3 * sizeof(float), GL_FALSE); // normals
    // // layout.addAttribute(GL_FLOAT, 2, 2 * sizeof(float), GL_FALSE); // texture coords

    // vao.addBuffer(std::move(vbo), layout);
    // vao.addBuffer(std::move(ebo));

    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glBindVertexArray(0);
}

void Mesh::draw(Shader &shader)
{
    // TODO: load and bind textures (waiting for lighting to be implemented first and shaders updated)
    // bind vao and draw using the ebo
    // draw mesh

    // vao.bind();
    // checkGLError("Mesh bound");
    // glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}