#include "rendering/buffer/buffer/buffer.h"
#include "utility"

Buffer::Buffer(GLenum targetType)
    : ID(0), targetType(targetType)
{
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

void Buffer::bind()
{
    glBindBuffer(targetType, ID);
}

void Buffer::unbind()
{
    glBindBuffer(targetType, 0);
}

void Buffer::assumeData(Buffer &&old)
{
    ID = old.ID;
    targetType = old.targetType;
    old.ID = 0;
    old.targetType = 0;
}