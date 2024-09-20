#include "root/graphics/implementation/vao.h"
#include "glad/glad.h"
#include <iostream>
#include "root/utils/logging.h"

Wombat::Graphics::VertexBufferElement::VertexBufferElement(GLenum type, unsigned int count, unsigned int totalSize, GLboolean normalised)
    : type(type), count(count), totalSize(totalSize), normalised(normalised)
{
}

Wombat::Graphics::VertexBufferElement::VertexBufferElement()
{
}

Wombat::Graphics::VertexBufferLayout::VertexBufferLayout()
    : attributeToElements(), stride()
{
}

void Wombat::Graphics::VertexBufferLayout::addAttribute(unsigned int index, GLenum type, unsigned int count, unsigned int totalSize, GLboolean normalised)
{
    Wombat::Graphics::VertexBufferElement vertexBufferElement = Wombat::Graphics::VertexBufferElement(type, count, totalSize, normalised);
    attributeToElements[index] = vertexBufferElement;
}

void Wombat::Graphics::VertexBufferLayout::addAttribute(GLenum type, unsigned int count, unsigned int totalSize, GLboolean normalised)
{
    addAttribute(attributeToElements.size(), type, count, totalSize, normalised);
}

const std::vector<Wombat::Graphics::VertexBufferElement> Wombat::Graphics::VertexBufferLayout::getElements() const
{
    std::vector<Wombat::Graphics::VertexBufferElement> elements;

    for (const auto &pair : attributeToElements)
    {
        elements.push_back(pair.second);
    }

    return elements;
}

unsigned int Wombat::Graphics::VertexBufferLayout::getStride() const
{
    unsigned int stride = 0;

    for (const auto &pair : attributeToElements)
    {
        stride += pair.second.totalSize;
    }

    return stride;
}

const std::map<int, Wombat::Graphics::VertexBufferElement> Wombat::Graphics::VertexBufferLayout::getMap() const
{
    return attributeToElements;
}

Wombat::Graphics::VAO::VAO()
{
    glGenVertexArrays(1, &vao_ID);
    LOG("Initialised new VAO: " + std::to_string(vao_ID), Logging::LOG_TYPE::INFO);
}

Wombat::Graphics::VAO::VAO(Wombat::Graphics::VAO &&other)
{
    // transfer ownership of Vertex Array Object to this VAO
    this->vao_ID = other.vao_ID;
    other.vao_ID = 0;
    // transfer VBOs and EBO
    this->vbos = std::move(other.vbos);
    this->ebo = std::move(other.ebo);
    other.vbos.clear();
    other.ebo = std::nullopt;
}

Wombat::Graphics::VAO &Wombat::Graphics::VAO::operator=(Wombat::Graphics::VAO &&other) noexcept
{
    if (this != &other)
    {
        // delete the current VAO as we are being assigned a new one
        glDeleteVertexArrays(1, &vao_ID);
        // transfer ownership of the Vertex Array Object to this VAO
        this->vao_ID = other.vao_ID;
        other.vao_ID = 0;
        // transfer VBOs and EBO
        this->vbos = std::move(other.vbos);
        this->ebo = std::move(other.ebo);
        other.vbos.clear();
        other.ebo = std::nullopt;
    }
    return *this;
}

Wombat::Graphics::VAO::~VAO()
{
    glDeleteVertexArrays(1, &vao_ID);
    if (vao_ID != 0)
        LOG("Deleted VAO: " + std::to_string(vao_ID), Logging::LOG_TYPE::INFO);
}

void Wombat::Graphics::VAO::addBuffer(Wombat::Graphics::VBO &&vbo, const VertexBufferLayout &layout)
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
    unsigned int vbo_id = vbo.getID();
    vbos.push_back(std::move(vbo)); // Store the VBO
    LOG(std::string("Assigned VBO: ") + std::to_string(vbo_id) + " to VAO: " + std::to_string(vao_ID), Logging::LOG_TYPE::INFO);
    unbind(); // Unbind the VAO
}

void Wombat::Graphics::VAO::addBuffer(EBO &&ebo)
{   
    unsigned int ebo_id = ebo.getID();
    this->ebo = std::move(ebo);
    LOG(std::string("Assigned EBO: ") + std::to_string(ebo_id) + " to VAO: " + std::to_string(vao_ID), Logging::LOG_TYPE::INFO);
}

void Wombat::Graphics::VAO::addVertexAttrribSpec(unsigned int attrib_ID, unsigned int count, GLenum type, GLboolean normalised, unsigned int stride, unsigned int offset)
{
    glEnableVertexAttribArray(attrib_ID);
    glVertexAttribPointer(attrib_ID, count, type, normalised, stride, reinterpret_cast<void *>(offset));
}

void Wombat::Graphics::VAO::bind() const
{
    glBindVertexArray(vao_ID);
    if (ebo.has_value())
    {
        ebo->bind();
    }
}

void Wombat::Graphics::VAO::unbind() const
{
    glBindVertexArray(0);
}