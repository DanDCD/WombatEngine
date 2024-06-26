#include "buffer/vao/vao.h"
#include "glad/glad.h"
#include <iostream>

VertexBufferElement::VertexBufferElement(unsigned int type, unsigned int count, unsigned int totalSize, bool normalised)
    : type(type), count(count), totalSize(totalSize), normalised(normalised)
{
}

VertexBufferElement::VertexBufferElement()
{
}

VertexBufferLayout::VertexBufferLayout()
    : attributeToElements(), stride()
{
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
    glGenVertexArrays(1, &vao_ID);
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
    bind();
    vbo.bind();
    unsigned int currentOffset = 0; // the offset so far
    for (const auto &pair : layout.getMap())
    {
        unsigned int attribute_id = pair.first;
        VertexBufferElement bufferElement = pair.second;

        glEnableVertexAttribArray(attribute_id);
        glVertexAttribPointer(attribute_id, bufferElement.count, bufferElement.type, bufferElement.normalised, layout.getStride(), reinterpret_cast<void *>(currentOffset));
        currentOffset += bufferElement.totalSize;
    }
    vbo.unbind();
    vbos.push_back(std::move(vbo));
    unbind();
}

void VAO::bind() const
{
    glBindVertexArray(vao_ID);
}

void VAO::unbind() const
{
    glBindVertexArray(0);
}