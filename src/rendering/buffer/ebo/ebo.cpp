#include "rendering/buffer/ebo/ebo.h"
#include <utility>

EBO::EBO()
    : Buffer(GL_ELEMENT_ARRAY_BUFFER) // EBOs should always target the elem arr buffer
{
}

EBO::EBO(EBO &&other)
    : Buffer(std::move(other))
{
}


EBO &EBO::operator=(EBO &&other) noexcept
{
    Buffer::operator=(std::move(other));
    return *this;
}