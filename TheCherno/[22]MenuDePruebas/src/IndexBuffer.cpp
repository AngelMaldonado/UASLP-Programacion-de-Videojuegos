/**
 * @brief A new C++ implementation file
 * 
 * @author @AngelMaldonado
 * @file IndexBuffer.cpp
 * @link www.somelink.com
 * @version 1.0 Fri Apr 22 2022
 */

#include "IndexBuffer.h"
#include "Renderizador.h"


IndexBuffer::IndexBuffer(const unsigned int* buffer, unsigned int numIndices) 
    : m_NumIndices(numIndices)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    GLCall(glGenBuffers(1, &m_IDRenderizador));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IDRenderizador));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), buffer, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() 
{
    GLCall(glDeleteBuffers(1, &m_IDRenderizador));
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IDRenderizador));
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
