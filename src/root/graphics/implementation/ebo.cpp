#include "root/graphics/implementation/ebo.h"
#include <utility>
#include "root/utils/logging.h"

EBO::EBO()
    : Buffer(GL_ELEMENT_ARRAY_BUFFER) // EBOs should always target the elem arr buffer
{
    LOG("Initialised new EBO: " + std::to_string(getID()), Logging::LOG_TYPE::INFO);
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