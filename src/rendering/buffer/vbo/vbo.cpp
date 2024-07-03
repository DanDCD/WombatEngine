#include "rendering/buffer/vbo/vbo.h"
#include <utility>

VBO::VBO(GLenum targetType)
    : Buffer(targetType)
{
}

VBO::VBO(VBO &&other)
    : Buffer(std::move(other))
{
}

VBO &VBO::operator=(VBO &&other) noexcept
{
    Buffer::operator=(std::move(other));
    return *this;
}