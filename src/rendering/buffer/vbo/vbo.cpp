#include "rendering/buffer/vbo/vbo.h"
#include <utility>
#include "utils/logging/logging.h"
#include <string>

VBO::VBO(GLenum targetType)
    : Buffer(targetType)
{
    LOG("Initialised new VBO: " + std::to_string(getID()), Logging::LOG_TYPE::INFO);
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