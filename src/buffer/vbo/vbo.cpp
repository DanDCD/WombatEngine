#include "buffer/vbo/vbo.h"

VBO::VBO()
{
    glGenBuffers(1, &vbo_ID);
    glGenBuffers(1, &ebo_ID);
}

VBO::VBO(VBO &&other)
{
    // obtain ownership of OpenGL VBO
    this->vbo_ID = other.vbo_ID;
    this->ebo_ID = other.ebo_ID;
    this->vboTargetType = other.vboTargetType;
    // remove ownership of VBO from old
    other.vbo_ID = 0;
    other.ebo_ID = 0;
    other.vboTargetType = 0;
}

VBO &VBO::operator=(VBO &&other) noexcept
{
    if (this != &other)
    {
        // delete the current VBO as we are being assigned a new one
        glDeleteBuffers(1, &vbo_ID);
        glDeleteBuffers(1, &ebo_ID);
        // transfer ownership of other's vbo to this one
        vbo_ID = other.vbo_ID;
        ebo_ID = other.ebo_ID;
        vboTargetType = other.vboTargetType;

        other.vbo_ID = 0;
        other.ebo_ID = 0;
        other.vboTargetType = 0;
    }
    return *this;
}

VBO::~VBO()
{
    glDeleteBuffers(1, &vbo_ID);
    glDeleteBuffers(1, &ebo_ID);
}

void VBO::assignVertData(GLenum targetType, const float *data, GLsizeiptr dataSize, GLenum usage)
{
    vboTargetType = targetType;
    bind();
    glBufferData(targetType, dataSize, data, usage);
    unbind();
}

void VBO::assignVertData(GLenum targetType, const float *data, GLsizeiptr dataSize, const unsigned int *indices, GLsizeiptr indicesSize, GLenum usage)
{
    vboTargetType = targetType;
    bind();
    glBufferData(targetType, dataSize, data, usage);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, usage);
    unbind();
}

void VBO::bind()
{
    glBindBuffer(vboTargetType, vbo_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_ID);
}

void VBO::unbind()
{
    glBindBuffer(vboTargetType, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int VBO::getVboID() const
{
    return vbo_ID;
}

unsigned int VBO::getEboId() const
{
    return ebo_ID;
}
