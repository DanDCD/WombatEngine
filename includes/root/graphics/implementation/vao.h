#pragma once
#include "root/graphics/implementation/vbo.h"
#include <glad/glad.h>
#include <map>
#include <vector>
#include <iostream>
#include <optional>
#include "root/graphics/implementation/ebo.h"

namespace Wombat
{
    namespace Graphics
    {

        class VertexBufferElement
        {
        public:
            /// @brief constructor
            /// @param type the type of the element
            /// @param count the number of these types
            /// @param totalSize size in bytes of this element
            /// @param normalised if these are normalised
            VertexBufferElement(GLenum type, unsigned int count, unsigned int totalSize, GLboolean normalised);

            /// @brief default constructor
            VertexBufferElement();

            /// @brief the OpenGL type of this element
            GLenum type;

            /// @brief the number of times this type appears contiguously
            unsigned int count;

            /// @brief the total size of this element in bytes (size of type * count)
            unsigned int totalSize;

            /// @brief if this type is a normalised type
            GLboolean normalised;
        };

        class VertexBufferLayout
        {

        public:
            VertexBufferLayout();

            void addAttribute(unsigned int index, GLenum type, unsigned int count, unsigned int totalSize, GLboolean normalised);

            void addAttribute(GLenum type, unsigned int count, unsigned int totalSize, GLboolean normalised);

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
            void addBuffer(Wombat::Graphics::VBO &&vbo, const VertexBufferLayout &layout);

            /// @brief add an Element Buffer Object to this VAO
            /// @param ebo the ebo to add
            void addBuffer(EBO &&ebo);

            /// @brief bind this VAO to OpenGL
            void bind() const;

            /// @brief unbind this VAO frrom OpenGL
            void unbind() const;

        private:
            /// @brief add a vertex attribute to this VAO
            /// @param attrib_ID
            /// @param count
            /// @param type
            /// @param normalised
            /// @param stride
            /// @param offset
            void addVertexAttrribSpec(unsigned int attrib_ID, unsigned int count, GLenum type, GLboolean normalised, unsigned int stride, unsigned int offset);

            /// @brief the id of the Vertex Array Object in OpenGL
            unsigned int vao_ID;

            /// @brief the vbos this vao owns
            std::vector<Wombat::Graphics::VBO> vbos;

            /// @brief the ebo this vao owns
            std::optional<EBO> ebo;
        };

    }
}