#pragma once
#include <glad/glad.h>


class EBO
{

public:
    /// @brief initialise Element Buffer Object in OpenGL
    EBO();

    /// @brief move constructor (e.g. EBO(std::move(oldEBO)))
    /// @param other the old EBO
    EBO(EBO &&other);

    /// @brief move assignment (e.g. vb2 = std::move(vb1))
    /// @param other the old EBO
    /// @return
    EBO &operator=(EBO &&other) noexcept;

    EBO(const EBO &) = delete; // dont allow vb2 = vb1

    EBO &operator=(const EBO &) = delete; // dont allow vb2 = EBO(vb1)

    /// @brief
    ~EBO();

    void assignIndiceData(const unsigned int *indices, GLsizeiptr indicesSize, GLenum usage);

    /// @brief bind the EBO and ebo to OpenGL
    void bind() const;

    /// @brief unbind the EBO and ebo from OpenGL
    void unbind();

    /// @brief get the id of the OpenGL Element Buffer Object
    /// @return the id
    unsigned int getEboId() const;

private:
    /// @brief the id of the ebo in OpenGL (may be unused if we dont use indices)
    unsigned int ebo_ID;
};