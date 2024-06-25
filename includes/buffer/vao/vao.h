#pragma once
#include "buffer/vbo/vbo.h"
#include <glad/glad.h>
#include <map>
#include <vector>

struct VertexBufferElement
{
    /// @brief constructor
    /// @param type the type of the element
    /// @param count the number of these types
    /// @param totalSize size in bytes of this element
    /// @param normalised if these are normalised
    VertexBufferElement(unsigned int type, unsigned int count, unsigned int totalSize, bool normalised);

    /// @brief the OpenGL type of this element
    unsigned int type;

    /// @brief the number of times this type appears contiguously
    unsigned int count;

    /// @brief the total size of this element in bytes (size of type * count)
    unsigned int totalSize;

    /// @brief if this type is a normalised type
    bool normalised;
};

class VertexBufferLayout
{

public:
    VertexBufferLayout();

    /// @brief push a new type to the layout
    /// @tparam T the c++ type to push
    /// @param index the index to be associated with this attribute
    /// @param count the number of elements to be pushed
    /// @param normalised whether fixed point values should be normalised or not
    template <typename T>
    void addAttribute(unsigned int index, unsigned int count, bool normalised);

    /// @brief push a new type to the layout (automatically assigns an index)
    /// @tparam T the c++ type to push
    /// @param count the number of elements to be pushed
    /// @param normalised whether fixed point values should be normalised or not
    template <typename T>
    void addAttribute(unsigned int count, bool normalised);

    /// @brief get the elements in the layout
    /// @return the VertexBufferElements represented in this layout
    const std::vector<VertexBufferElement> getElements() const;

    /// @brief return the total stride of this layout
    /// @return the stride in bytes
    unsigned int getStride() const;

    /// @brief returns a mapping between attribute ID and buffer element
    /// @return the mappings in this layout
    const std::map<int, VertexBufferElement> getMap() const;

private:
    /// @brief maps vertex attributes to the elements that structure them
    std::map<int, VertexBufferElement> attributeToElements;

    /// @brief returns the total stride of this layout
    unsigned int stride;
};

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

    /// @brief add a Virtual Buffer Object to this VAO in OpenGL
    /// @param vbo the vbo to add
    /// @param layout the layout of this vbo
    void addVBO(VBO &&vbo, const VertexBufferLayout &layout);

private:
    /// @brief the id of the Vertex Array Object in OpenGL
    unsigned int vao_ID;

    std::vector<VBO> vbos;
};