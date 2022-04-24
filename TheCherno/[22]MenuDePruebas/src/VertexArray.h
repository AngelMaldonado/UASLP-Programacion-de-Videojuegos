/**
 * @brief A new C++ header file
 * 
 * @author @AngelMaldonado
 * @file VertexArray.h
 * @link www.somelink.com
 * @version 1.0 Fri Apr 22 2022
 */

#pragma once

#include "VertexBuffer.h"

class DistribucionVertexBuffer;

class VertexArray
{
private:
    /**
     * @brief ID del VertexArray
     */
    unsigned int m_IDRenderizador;
public:
    /**
     * @brief Constructor del VertexArray crea el vao y asigna un ID
     */
    VertexArray();

    /**
     * @brief Destructor del VertexArray elimina el buffer del vao
     */
    ~VertexArray();

    /**
     * @brief Funcion para agregar un buffer al VertexArray con su distribucion ya especificada
     * @param buffer Buffer a agregar
     * @param distribucion Distribucion de los datos en el buffer
     * @return Enlaza el buffer de datos con la distribucion especificada en el parametro dentro
     * de OpenGL
     */
    void AgregaBuffer(const VertexBuffer& vb, const DistribucionVertexBuffer& distribucion);

    /**
     * @brief Funcion para activar el VertexArray
     */
    void Bind() const;

    /**
     * @brief Funcion para desactivar el VertexArray
     */
    void Unbind() const;
};

