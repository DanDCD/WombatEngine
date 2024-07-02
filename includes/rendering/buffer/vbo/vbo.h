#pragma once
#include <vector>
#include "rendering/buffer/buffer/buffer.h"
#include "glad/glad.h"

class VBO
{

public:
    /// @brief initialise Virtual Buffer Object in OpenGL
    VBO();

    /// @brief move constructor (e.g. VBO(std::move(oldVbo)))
    /// @param other the old vbo
    VBO(VBO &&other);

    /// @brief move assignment (e.g. vb2 = std::move(vb1))
    /// @param other the old vbo
    /// @return
    VBO &operator=(VBO &&other) noexcept;

    VBO(const VBO &) = delete; // dont allow vb2 = vb1

    VBO &operator=(const VBO &) = delete; // dont allow vb2 = VBO(vb1)

    /// @brief
    ~VBO();

    /// @brief assign Vertex data to this VBO
    /// @param targetType the type of buffer we want to assign to
    /// @param data the vertex data being assigned
    /// @param dataSize size of data in bytes
    /// @param usage how this data is expected to be accessed
    void assignVertData(GLenum targetType, const float *data, GLsizeiptr dataSize, GLenum usage);

    /// @brief bind the vbo and ebo to OpenGL
    void bind();

    /// @brief unbind the vbo and ebo from OpenGL
    void unbind();

    /// @brief get the id of the OpenGL Vertex Buffer Object
    /// @return  the id
    unsigned int getVboID() const;

private:
    /// @brief the id of this vbo in OpenGL
    unsigned int vbo_ID;

    /// @brief the target type of the vbo
    GLenum vboTargetType;
};
