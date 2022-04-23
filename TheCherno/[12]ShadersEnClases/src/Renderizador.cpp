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
