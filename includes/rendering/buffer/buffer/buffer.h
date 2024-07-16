#pragma once
#include "glad/glad.h"
#include "rendering/vertex/vertex.h"

/// @brief encapsulates an OpenGL buffer: managing creation, deletion, and basic buffer operations
class Buffer
{
public:
    /// @brief initialise Buffer Object in OpenGL
    /// @param targetType the type of buffer we want to associate with
    Buffer(GLenum targetType);

    /// @brief move constructor (e.g. Buffer(std::move(oldBuffer)))
    /// @param other the old buffer
    Buffer(Buffer &&other);

    /// @brief move assignment (e.g. b2 = std::move(b1))
    /// @param other the old buffer
    /// @return
    Buffer &operator=(Buffer &&other) noexcept;

    Buffer(const Buffer &) = delete; // dont allow b2 = b1;

    Buffer &operator=(const Buffer &) = delete; // dont allow b2 = Buffer(b1)

    /// @brief the Buffer destructor will remove the Buffer from OpenGL too
    ~Buffer();

    void assignData(const float *data, GLsizeiptr dataSize, GLenum usage);

    void assignData(const unsigned int *data, GLsizeiptr dataSize, GLenum usage);

    void assignData(const Vertex *data, GLsizeiptr dataSize, GLenum usage);

    /// @brief binds this buffer to OpenGL
    void bind() const;

    /// @brief unbinds this buffer from OpenGL
    void unbind() const;

    /// @brief get the id of this buffer
    /// @return the id
    unsigned int getID();

private:
    /// @brief take over data of old buffer
    /// @param old the buffer to take control of data from (i.e. the buffer pointed to)
    void assumeData(Buffer &&old);

    /// @brief the id of this buffer in OpenGL
    unsigned int ID;

    /// @brief the buffer type of this buffer in OpenGL
    GLenum targetType;
};