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

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

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

class Renderizador
{
public:
    /**
     * @brief Funcion para limpiar el buffer de colores
     */
    void LimpiaBuffer() const;

    /**
     * @brief Funcion para dibujar los buffers en pantalla
     * @param va VertexArray - Objeto que contiene los buffers
     * @param ib IndexBuffer - Objeto que contiene los indices para los datos en el buffer
     * @param shader Shader - Objeto que contiene los shaders para el renderizado
     * @return Muestra en pantalla el resultado de la renderizacion
     */
    void Dibuja(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
private:
};
