/**
 * @brief A new C++ header file
 * 
 * @author @AngelMaldonado
 * @file DistribucionVertexBuffer.h
 * @link www.somelink.com
 * @version 1.0 Fri Apr 22 2022
 * 
 * TODO: Hay un error con vs code en la especializacion de las plantillas para el metodo Push
 */

#pragma once

#include <GL/glew.h>
#include <vector>

#include "Renderizador.h"

/**
 * @brief Estructura para almacenar un elemento dentro de la distribucion
 * de un buffer (std::vector<ElementoVertexBuffer> m_elementos)
 * @param tipo tipo de dato que tiene el buffer (GL_FLOAT, GL_UNSIGNED_INT, etc)
 * @param cuenta cantidad de datos que tiene el buffer (coordenadas, normales, etc)
 * @param normalizado si los datos deben ser normalizados o no (0.0 - 1.0)
 */
struct ElementoVertexBuffer
{
    /* Cantidad de datos que tiene el buffer (2 coordenadas, 2 normales, etc) */
    unsigned int tipo;
    /* Tipo de dato del buffer */
    unsigned int cuenta;
    /* Si los datos deben ser normalizados o no */
    unsigned char normalizado;

    /* Funcion para obtener el tamanio del tipo de dato del buffer */
    static unsigned int ObtenTamTipo(unsigned int tipo)
    {
        switch (tipo)
        {
            case GL_FLOAT:          return sizeof(float);
            case GL_UNSIGNED_INT:   return sizeof(unsigned int);
            case GL_UNSIGNED_BYTE:  return sizeof(unsigned char);
        }
        ASSERT(false);
        return 0;
    }
};

class DistribucionVertexBuffer
{
private:
    /**
     * @brief Vector con la informacion de la distribucion del buffer
     * (ElementoVertexBuffer)
     */
    std::vector<ElementoVertexBuffer> m_Elementos;
    /**
     * @brief Offset de la distribucion del buffer (bytes entre cada vertice)
     */
    unsigned int m_Stride;
public:
    /**
     * @brief Constructor del VertexArray
     */
    DistribucionVertexBuffer()
        : m_Stride(0) {}

    template<typename T>
    void Push(unsigned int cuenta)
    {
        ASSERT(false);
    }
    
    /**
     * @brief Obten la informacion de distribucion del buffer
     */
    inline const std::vector<ElementoVertexBuffer> ObtenElementos() const& { return m_Elementos; } 
    /**
     * @brief Obten el offset de la distribucion del buffer (bytes entre cada vertice)
     */
    inline unsigned int ObtenStride() const { return m_Stride; }
};

template<>
inline void DistribucionVertexBuffer::Push<float>(unsigned int cuenta)
{
    m_Elementos.push_back({ GL_FLOAT, cuenta, GL_FALSE });
    m_Stride += cuenta * ElementoVertexBuffer::ObtenTamTipo(GL_FLOAT);
}

template<>
inline void DistribucionVertexBuffer::Push<unsigned int>(unsigned int cuenta)
{
    m_Elementos.push_back({ GL_UNSIGNED_INT, cuenta, GL_FALSE });
    m_Stride += cuenta * ElementoVertexBuffer::ObtenTamTipo(GL_UNSIGNED_INT);
}

template<>
inline void DistribucionVertexBuffer::Push<unsigned char>(unsigned int cuenta)
{
    m_Elementos.push_back({ GL_UNSIGNED_BYTE, cuenta, GL_TRUE });
    m_Stride += cuenta * ElementoVertexBuffer::ObtenTamTipo(GL_UNSIGNED_BYTE);
}
