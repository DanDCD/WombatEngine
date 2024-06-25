#include "buffer/vao/vao.h"
#include "glad/glad.h"
#include <iostream>

VertexBufferElement::VertexBufferElement(unsigned int type, unsigned int count, unsigned int totalSize, bool normalised)
    : type(type), count(count), normalised(normalised)
{
}

VertexBufferLayout::VertexBufferLayout()
    : attributeToElements(), stride()
{
}

template <typename T>
void VertexBufferLayout::addAttribute(unsigned int index, unsigned int count, bool normalised)
{
    unsigned int type;
    unsigned int totalSize;

    if constexpr (std::is_same_v<T, int>)
    {
        type = GL_INT;
        totalSize = count * sizeof(int);
    }
    else if constexpr (std::is_same_v<T, float>)
    {
        type = GL_FLOAT;
        totalSize = count * sizeof(float);
    }
    else if constexpr (std::is_same_v<T, bool>)
    {
        type = GL_BOOL;
        totalSize = count * sizeof(bool);
    }
    else
    {
        std::cerr << "ERROR::VERTEX_BUFFER_LAYOUT::UNSUPPORTED_TYPE" << std::endl;
        return;
    }

    VertexBufferElement vertexBufferElement(type, count, totalSize, normalised);
    attributeToElements[index] = vertexBufferElement;
}

template <typename T>
void VertexBufferLayout::addAttribute(unsigned int count, bool normalised)
{
    addAttribute<T>(attributeToElements.size(), count, normalised);
}

const std::vector<VertexBufferElement> VertexBufferLayout::getElements() const
{
    std::vector<VertexBufferElement> elements;

    for (const auto &pair : attributeToElements)
    {
        elements.push_back(pair.second);
    }

    return elements;
}

unsigned int VertexBufferLayout::getStride() const
{
    unsigned int stride = 0;

    for (const auto &pair : attributeToElements)
    {
        stride += pair.second.totalSize;
    }

    return stride;
}

const std::map<int, VertexBufferElement> VertexBufferLayout::getMap() const
{
    return attributeToElements;
}

VAO::VAO()
{
}

VAO::VAO(VAO &&other)
{
    // transfer ownership of Vertex Array Object to this VAO
    this->vao_ID = other.vao_ID;
    other.vao_ID = 0;
}

VAO &VAO::operator=(VAO &&other) noexcept
{
    if (this != &other)
    {
        // delete the current VAO as we are being assigned a new one
        glDeleteVertexArrays(1, &vao_ID);
        // transfer ownership of the Vertex Array Object to this VAO
        this->vao_ID = other.vao_ID;
        other.vao_ID = 0;
    }
    return *this;
}

VAO::~VAO()
{
    glDeleteVertexArrays(1, &vao_ID);
}

void VAO::addVBO(VBO &&vbo, const VertexBufferLayout &layout)
{
    vbo.bind();
    unsigned int currentOffset = 0; // the offset so far
    for (const auto &pair : layout.getMap())
    {
        unsigned int attribute_id = pair.first;
        VertexBufferElement bufferElement = pair.second;

        glEnableVertexAttribArray(attribute_id);
        glVertexAttribPointer(attribute_id, bufferElement.count, bufferElement.type, bufferElement.normalised, layout.getStride(), (void *)(currentOffset));
        currentOffset += bufferElement.totalSize;
    }
    vbos.push_back(vbo);
    vbo.unbind();
}