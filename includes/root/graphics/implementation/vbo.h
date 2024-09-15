#pragma once
#include "root/graphics/implementation/buffer.h"
#include "glad/glad.h"

/// @brief the Virtual Buffer Object enables the storage of vertex data in OpenGL
class VBO : public Buffer
{
public:
    VBO(GLenum targetType);

    VBO(VBO &&other);

    VBO &operator=(VBO &&other) noexcept;
};