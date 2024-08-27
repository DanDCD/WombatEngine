#include "rendering/log/check_gl.h"
#include <glad/glad.h>
#include <iostream>
#include "utils/logging/logging.h"

void checkGLError(const std::string &label)
{
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        LOG("OpenGL error detected in check at label [" + label + "]: " + std::to_string(err), Logging::LOG_TYPE::ERROR);
    }
}