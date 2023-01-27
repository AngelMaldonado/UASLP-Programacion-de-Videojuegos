/**
 * @brief A new C++ header file
 * 
 * @author @AngelMaldonado
 * @file IndexBuffer.h
 * @link www.somelink.com
 * @version 1.0 Fri Apr 22 2022
 */

#pragma once

class IndexBuffer
{
private:
    /**
     * @brief ID del VertexBuffer (apuntador a los datos)
     */
    unsigned int m_IDRenderizador;

    /**
     * @brief Contador de cuantos indices hay en el buffer
     */
    unsigned int m_NumIndices;
public:
    /**
     * @brief Constructor del VertexBuffer
     * @param buffer Buffer de datos a renderizar/bindear
     * @param tam Tamaño del buffer
     * @return Objeto VertexBuffer creado
     */
    IndexBuffer(const unsigned int* buffer, unsigned int numIndices);

    /**
     * @brief Destructor del VertexBuffer, destruye el buffer m_IDRenderizador
     */
    ~IndexBuffer();

    /**
     * @brief Funcion para hacer bind del buffer m_IDRenderizador
     */
    void Bind() const;

    /**
     * @brief Funcion para hacer un unbind del buffer, hace bind del buffer 0
     */
    void Unbind() const;

    /**
     * @brief Funcion para obtener el numero de indices del buffer
     * @return Numero de indices del buffer
     */
    inline unsigned int GetNumIndices() const { return m_NumIndices; };
};
