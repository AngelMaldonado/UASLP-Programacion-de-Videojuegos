/**
 * @brief A new C++ header file
 * 
 * @author @AngelMaldonado
 * @file VertexBuffer.h
 * @link www.somelink.com
 * @version 1.0 Fri Apr 22 2022
 */

#pragma once

class VertexBuffer 
{
private:
    /**
     * @brief ID del VertexBuffer (apuntador a los datos)
     */
    unsigned int m_IDRenderizador;
public:
    /**
     * @brief Constructor del VertexBuffer
     * @param buffer Buffer de datos a renderizar/bindear
     * @param tam Tamaño del buffer
     * @return Objeto VertexBuffer creado
     */
    VertexBuffer(const void* buffer, unsigned int tam);

    /**
     * @brief Destructor del VertexBuffer, destruye el buffer m_IDRenderizador
     */
    ~VertexBuffer();

    /**
     * @brief Funcion para hacer bind del buffer m_IDRenderizador
     */
    void Bind() const;

    /**
     * @brief Funcion para hacer un unbind del buffer, hace bind del buffer 0
     */
    void Unbind() const;
};
