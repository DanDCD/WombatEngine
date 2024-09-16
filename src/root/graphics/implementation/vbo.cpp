#include "root/graphics/implementation/vbo.h"
#include <utility>
#include "root/utils/logging.h"
#include <string>

Wombat::Graphics::VBO::VBO(GLenum targetType)
    : Buffer(targetType)
{
    LOG("Initialised new VBO: " + std::to_string(getID()), Logging::LOG_TYPE::INFO);
}

Wombat::Graphics::VBO::VBO(Wombat::Graphics::VBO &&other)
    : Buffer(std::move(other))
{
}

Wombat::Graphics::VBO &Wombat::Graphics::VBO::operator=(Wombat::Graphics::VBO &&other) noexcept
{
    Buffer::operator=(std::move(other));
    return *this;
}