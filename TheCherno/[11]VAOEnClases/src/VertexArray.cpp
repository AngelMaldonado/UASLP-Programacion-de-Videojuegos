/**
 * @brief A new C++ implementation file
 * 
 * @author @AngelMaldonado
 * @file VertexArray.cpp
 * @link www.somelink.com
 * @version 1.0 Fri Apr 22 2022
 */

#include "VertexArray.h"
#include "Renderizador.h"

VertexArray::VertexArray() 
{
    GLCall(glGenVertexArrays(1, &m_IDRenderizador));
    GLCall(glBindVertexArray(m_IDRenderizador));
}

VertexArray::~VertexArray() 
{
    GLCall(glDeleteVertexArrays(1, &m_IDRenderizador));
}

void VertexArray::AgregaBuffer(const VertexBuffer& vb, const DistribucionVertexBuffer& distribucion) 
{
    Bind();
    vb.Bind();
    const auto& elementos = distribucion.ObtenElementos();
    unsigned int offset = 0; // bytes entre cada dato del buffer 'elemento'
    for (unsigned int i = 0; i < elementos.size(); i++)
    {
        const auto& elemento = elementos[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, elemento.cuenta, elemento.tipo, elemento.normalizado, distribucion.ObtenStride(), (const void*)(intptr_t)offset);
        offset += elemento.cuenta * ElementoVertexBuffer::ObtenTamTipo(elemento.tipo);
    }
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
}

void VertexArray::Bind() 
{
    GLCall(glBindVertexArray(m_IDRenderizador));
}

void VertexArray::Unbind() 
{
    GLCall(glBindVertexArray(0));
}
