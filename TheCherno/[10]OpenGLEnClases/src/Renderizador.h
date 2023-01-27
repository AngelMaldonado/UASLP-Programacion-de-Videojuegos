/**
 * @brief A new C++ header file
 * 
 * @author @AngelMaldonado
 * @file Renderizador.h
 * @link glew: https://glew.sourceforge.io/ (7.0)
 * @version 1.0 Fri Apr 22 2022
 */

#pragma once

#include <GL/glew.h>

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

/**
 * @brief Funcion para limpiar el buffer de errores
 */
void GLClearError();

/**
 * @brief Funcion para mostrar los errores de OpenGL
 * @param funcion Funcion donde se evaluan los errores
 * @param archivo Archivo donde se encuentra la funcion
 * @param linea Linea donde se encuentra la funcion
 */
bool GLLogCall(const char* funcion, const char* archivo, int linea);
