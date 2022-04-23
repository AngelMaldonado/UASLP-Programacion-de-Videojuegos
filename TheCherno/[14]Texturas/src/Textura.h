/**
 * @brief A new C++ header file
 * 
 * @author @AngelMaldonado
 * @file Textura.h
 * @link www.somelink.com
 * @version 1.0 Fri Apr 22 2022
 */

#pragma once

#include <string>

class Textura
{
private:
    /**
     * @brief ID de la textura (apuntador a los datos de las texturas)
     */
    unsigned int m_IDRenderizador;

    /**
     * @brief Cadena con la ruta del archivo de la textura
     */
    std::string m_RutaArchivo;

    /**
     * @brief Almacenamiento local para el buffer con las coordenadas de textura
     */
    unsigned char* m_BufferLocal;

    /**
     * @brief Ancho de la textura (imagen)
     */
    int m_Ancho;

    /**
     * @brief Alto de la textura (imagen)
     */
    int m_Alto;

    /**
     * @brief Bits por pixel de la textura (imagen)
     */
    int m_BitsPorPixel;
public:
    /**
     * @brief Constructor de la clase Textura
     * @param rutaArchivo Cadena con la ruta del archivo de la textura
     */
    Textura(const std::string& rutaArchivo);

    /**
     * @brief Destructor de la clase Textura
     */
    ~Textura();

    /**
     * @brief Activar la textura (bind)
     * @param ranura Espacio de textura que se va a utilizar/cargar, la cantidad
     * de ranuras de textura disponibles depende de la tarjeta gráfica y del dispositivo.
     * Si no se especifica la ranura, se elige por defecto la 0.
     */
    void Bind(unsigned int ranura = 0) const;

    /**
     * @brief Desactivar la textura (unbind)
     */
    void Unbind() const;

    /**
     * @brief Funcion para obtener el ancho de la imagen
     */
    inline int GetAncho() const { return m_Ancho; }

    /**
     * @brief Funcion para obtener el alto de la imagen
     */
    inline int GetAlto() const { return m_Alto; }
};
