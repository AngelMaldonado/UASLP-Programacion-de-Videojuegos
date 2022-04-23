/**
 * @brief A new C++ implementation file
 * 
 * @author @AngelMaldonado
 * @file Textura.cpp
 * @link www.somelink.com
 * @version 1.0 Fri Apr 22 2022
 */

#include "Textura.h"
#include "vendor/stb_image/stb_image.h"
#include "Renderizador.h"

Textura::Textura(const std::string& rutaArchivo)
    : m_IDRenderizador(0), m_RutaArchivo(rutaArchivo), m_BufferLocal(nullptr),
    m_Ancho(0), m_Alto(0), m_BitsPorPixel(0)
{
    /* OpenGL obtiene las coordenadas desde abajo-izquierda, por lo que se debe girar la imagen */
    stbi_set_flip_vertically_on_load(1);
    m_BufferLocal = stbi_load(rutaArchivo.c_str(), &m_Ancho, &m_Alto, &m_BitsPorPixel, 4); // 4 = RGBA

    GLCall(glGenTextures(1, &m_IDRenderizador));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_IDRenderizador));

    /* Establece los parametros para las texturas */
    /* Filtros minimo y maximo para escalar la imagen/textura de manera linear */
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    /* Cargar la textura con OpenGL */
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Ancho, m_Alto, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_BufferLocal));
    /* Reinicia el apuntador (unbind) */
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    /* Liberar el buffer local */
    if(m_BufferLocal)
        stbi_image_free(m_BufferLocal);
}

Textura::~Textura() 
{
    GLCall(glDeleteTextures(1, &m_IDRenderizador));
}

void Textura::Bind(unsigned int ranura) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + ranura));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_IDRenderizador));
}

void Textura::Unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
