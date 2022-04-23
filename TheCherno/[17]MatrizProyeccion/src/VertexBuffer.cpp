/**
 * @brief A new C++ implementation file
 * 
 * @author @AngelMaldonado
 * @file VertexBuffer.cpp
 * @link www.somelink.com
 * @version 1.0 Fri Apr 22 2022
 */

#include "VertexBuffer.h"
#include "Renderizador.h"


VertexBuffer::VertexBuffer(const void* buffer, unsigned int tam) 
{
    GLCall(glGenBuffers(1, &m_IDRenderizador));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_IDRenderizador));
    GLCall(glBufferData(GL_ARRAY_BUFFER, tam, buffer, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() 
{
    GLCall(glDeleteBuffers(1, &m_IDRenderizador));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_IDRenderizador));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
