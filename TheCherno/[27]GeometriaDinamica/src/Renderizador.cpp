/**
 * @brief A new C++ implementation file
 * 
 * @author @AngelMaldonado
 * @file Renderizador.cpp
 * @version 1.0 Fri Apr 22 2022
 */

#include <iostream>

#include "Renderizador.h"

void GLClearError()
{
    while(glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* funcion, const char* archivo, int linea)
{
    while(GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] ( " << error << " ): " << funcion <<
            " " << archivo << ":" << linea << std::endl;
        return false;
    }
    return true;
}

void Renderizador::LimpiaBuffer() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderizador::Dibuja(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetNumIndices(), GL_UNSIGNED_INT, nullptr));

    // Unbind
}
