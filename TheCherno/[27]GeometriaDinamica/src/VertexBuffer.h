/**
 * @brief A new C++ header file
 * 
 * @author @AngelMaldonado
 * @file VertexBuffer.h
 * @link www.somelink.com
 * @version 1.0 Fri Apr 22 2022
 */

#pragma once

/**
 * @brief Esta clase es la encargada de definir el buffer principal en donde se
 * almacenan los datos que utiliza la GPU para renderizar, estos datos son principalmente
 * datos flotantes que representan las coordenadas de vertices, valores de color, coordenadas
 * de texturas, etc.
 * 
 * Al crear una instancia de esta clase se define un espacio especifico para una determinada
 * cantidad de vertices (por defecto 1000 vertices), sin embargo, no se le pasa ningun tipo de 
 * informacion hasta que se le indique, por lo que la informacion que guarde en el buffer puede 
 * cambiar constantemente.
 * 
 */
class VertexBuffer 
{
private:
    /**
     * @brief ID del VertexBuffer (apuntador a los datos)
     */
    unsigned int m_IDRenderizador;
public:
    /**
     * @brief Constructor dinamico para el VertexBuffer, la cantidad de bytes para el
     * buffer dependera de la cantidad de bytes que se le pasen por parametro. Ej:
     * VertexBuffer(sizeof(Vertice) * 1000);
     * Esto prepara un VB con espacio para 1000 instancias de Vertice
     * @param bufferTotal
     * @return Objeto VertexBuffer creado con una determinada cantidad de espacio
     * dinamico.
     */
    VertexBuffer(unsigned int bufferTotal);

    /**
     * @brief Constructor estatico para el VertexBuffer
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
     * @brief Funcion para hacer bind dinamico del buffer m_IDRenderizador, esta
     * informacion del buffer puede ser actualizada constantemente.
     * @param buffer Buffer con los datos para agregar al VB, este buffer puede ser
     * modificado ya que en el ciclo principal se actualiza constantemente.
     * @param tamBuffer Tamano del buffer en bytes
     * @return Agrega al VB la informacion del buffer del parametro
     */
    void BindDinamico(const void* buffer, unsigned int tamBuffer) const;

    /**
     * @brief Funcion para hacer bind del buffer m_IDRenderizador
     */
    void Bind() const;

    /**
     * @brief Funcion para hacer un unbind del buffer, hace bind del buffer 0
     */
    void Unbind() const;
};
