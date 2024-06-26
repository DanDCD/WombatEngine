#include "buffer/ebo/ebo.h"

EBO::EBO()
{
    glGenBuffers(1, &ebo_ID);
}

EBO::EBO(EBO &&other)
{
    ebo_ID = other.ebo_ID;
    other.ebo_ID = 0;
}

EBO &EBO::operator=(EBO &&other) noexcept
{
    if (this != &other)
    {
        // delete the current ebo as we are being assigned a new one
        glDeleteBuffers(1, &ebo_ID);
        // obtain ownership of others ebo
        ebo_ID = other.ebo_ID;
        other.ebo_ID = 0;
    }
    return *this;
}

EBO::~EBO()
{
    glDeleteBuffers(1, &ebo_ID);
}

void EBO::assignIndiceData(const unsigned int *indices, GLsizeiptr indicesSize, GLenum usage)
{
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, usage);
    unbind();
}

void EBO::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_ID);
}

void EBO::unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}