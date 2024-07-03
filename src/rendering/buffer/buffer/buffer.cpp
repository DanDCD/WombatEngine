#include "rendering/buffer/buffer/buffer.h"
#include "utility"



Buffer::Buffer(GLenum targetType)
    : ID(0), targetType(targetType)
{
    glGenBuffers(1, &ID);
}

Buffer::Buffer(Buffer &&other)
{
    // take over ownership of other's buffer
    assumeData(std::move(other));
}

Buffer &Buffer::operator=(Buffer &&other) noexcept
{
    if (this != &other)
    {
        // delete current buffer as its being replaced
        glDeleteBuffers(1, &ID);
        // take over ownership of other's buffer
        assumeData(std::move(other));
    }
    return *this;
}

Buffer::~Buffer()
{
    glDeleteBuffers(1, &ID);
}

void Buffer::assignData(const float *data, GLsizeiptr dataSize, GLenum usage)
{
    bind();
    glBufferData(targetType, dataSize, data, usage);
    unbind();
}

void Buffer::assignData(const unsigned int *data, GLsizeiptr dataSize, GLenum usage)
{
    bind();
    glBufferData(targetType, dataSize, data, usage);
    unbind();
}

void Buffer::bind() const
{
    glBindBuffer(targetType, ID);
}

void Buffer::unbind() const
{
    glBindBuffer(targetType, 0);
}

unsigned int Buffer::getID()
{
    return ID;
}
void Buffer::assumeData(Buffer &&old)
{
    ID = old.ID;
    targetType = old.targetType;
    old.ID = 0;
    old.targetType = 0;
}