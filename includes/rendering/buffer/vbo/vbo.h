#pragma once
#include <vector>
#include "rendering/buffer/buffer/buffer.h"
#include "glad/glad.h"

class VBO : public Buffer
{
public:
    VBO(GLenum targetType);

    VBO(VBO &&other);

    VBO &operator=(VBO &&other) noexcept;
};