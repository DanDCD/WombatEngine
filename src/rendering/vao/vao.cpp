#include "rendering/vao/vao.h"
#include "glad/glad.h"
#include <iostream>
#include "rendering/buffer/ebo/ebo.h"

VertexBufferElement::VertexBufferElement(GLenum type, unsigned int count, unsigned int totalSize, GLboolean normalised)
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

void VertexBufferLayout::addAttribute(unsigned int index, GLenum type, unsigned int count, unsigned int totalSize, GLboolean normalised)
{
    VertexBufferElement vertexBufferElement = VertexBufferElement(type, count, totalSize, normalised);
    attributeToElements[index] = vertexBufferElement;
}

void VertexBufferLayout::addAttribute(GLenum type, unsigned int count, unsigned int totalSize, GLboolean normalised)
{
    addAttribute(attributeToElements.size(), type, count, totalSize, normalised);
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

        addVertexAttrribSpec(attribute_id, bufferElement.count, bufferElement.type, bufferElement.normalised, stride, currentOffset);

        // Increment the offset
        currentOffset += bufferElement.totalSize;
    }

    vbo.unbind();                   // Unbind the VBO
    vbos.push_back(std::move(vbo)); // Store the VBO
    unbind();                       // Unbind the VAO
}

void VAO::addEBO(EBO &&ebo)
{
    this->ebo = std::move(ebo);
}

void VAO::addVertexAttrribSpec(unsigned int attrib_ID, unsigned int count, GLenum type, GLboolean normalised, unsigned int stride, unsigned int offset)
{
    glEnableVertexAttribArray(attrib_ID);
    glVertexAttribPointer(attrib_ID, count, type, normalised, stride, reinterpret_cast<void *>(offset));
}

void VAO::bind() const
{
    glBindVertexArray(vao_ID);
    if(ebo.has_value()){
        ebo->bind();
    }
}

void VAO::unbind() const
{
    glBindVertexArray(0);
}