#include "buffer/vao/vao.h"
#include "glad/glad.h"
#include <iostream>

VertexBufferElement::VertexBufferElement(unsigned int type, unsigned int count, unsigned int totalSize, GLboolean normalised)
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
    bind();     // Bind the VAO
    vbo.bind(); // Bind the VBO

    unsigned int stride = layout.getStride(); // Get the stride from the layout
    unsigned int currentOffset = 0;           // Initialize the offset

    // Iterate through the layout elements
    for (const auto &pair : layout.getMap())
    {
        unsigned int attribute_id = pair.first;
        const VertexBufferElement &bufferElement = pair.second;

        glEnableVertexAttribArray(attribute_id); // Enable the vertex attribute

        // Set the vertex attribute pointer
        glVertexAttribPointer(attribute_id, bufferElement.count, bufferElement.type,
                              bufferElement.normalised, stride,
                              reinterpret_cast<void *>(currentOffset));

        // Increment the offset
        currentOffset += bufferElement.totalSize;
    }

    vbo.unbind();                   // Unbind the VBO
    vbos.push_back(std::move(vbo)); // Store the VBO
    unbind();                       // Unbind the VAO
}

void VAO::addVertexAttrribSpec(unsigned int attrib_ID, unsigned int count, GLenum type, GLboolean normalised, unsigned int stride, unsigned int offset)
{
    glVertexAttribPointer(attrib_ID, count, type, normalised, stride, reinterpret_cast<void *>(offset));
    glEnableVertexAttribArray(attrib_ID);
}

void VAO::bind() const
{
    glBindVertexArray(vao_ID);
}

void VAO::unbind() const
{
    glBindVertexArray(0);
}