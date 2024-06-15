#pragma once
#include "buffer/vbo/vbo.h"

class VAO
{

public:
    /// @brief construct VAO and create a vertex atrribute object in OpenGL
    VAO();

    /// @brief the move constructor for VAO - used when we want to transfer ownership of the VAO data between variables (copy constructor for rvalues) (e.g. VAO(std::move(oldVAO)))
    /// @param other the old VAO to be moved into this one
    VAO(VAO &&other);

    /// @brief the move assignment for VAO - used when we want to transfer ownership of the VAO data between variables (copy assignment for rvalues) (e.g. vao1 = std::move(vao2))
    /// @param other
    VAO &operator=(VAO &&other) noexcept;

    /// @brief prevents copy constructor from lvalues (e.g. vao = VAO(oldVAO); or return vao;)
    VAO(const VAO &) = delete;

    /// @brief prevents copy assignment from lvalues (e.g. vao1 = vao2;)
    VAO &operator=(const VAO &) = delete;

    /// @brief destructor - deletes VAO from OpenGL
    ~VAO();

    void addVBO(VBO &vbo);

private:
    /// @brief the id of the Vertex Array Object in OpenGL
    unsigned int vao_ID;
};