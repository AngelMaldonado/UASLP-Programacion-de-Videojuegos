/**
 * @brief A new C++ header file
 * 
 * @author @AngelMaldonado
 * @file Shader.h
 * @link www.somelink.com   
 * @version 1.0 Fri Apr 22 2022
 */

#pragma once

#include <string>
#include <unordered_map>

struct CodigoFuenteShader {
    std::string CodigoVertex;
    std::string CodigoFragment;
};

class Shader
{
private:
    /**
     * @brief ID del shader (apuntador a los datos)
     */
    unsigned int m_IDRenderizador;

    /**
     * @brief Ruta del archivo del shader
     */
    std::string m_rutaArchivoShader;

    /**
     * @brief Mapa de direcciones de los uniformes
     */
    std::unordered_map<std::string, int> m_cacheDireccionesUniformes;
public:
    /**
     * @brief Constructor de la clase Shader. Procesa el codigo fuente del shader y
     * lo guarda en la estructura CodigoFuenteShader, a su vez carga el programa en OpenGL.
     * @param rutaArchivoShader Ruta del archivo del shader.
     * @return Genera un ID para el shader en memoria.
     */
    Shader(const std::string& rutaArchivoShader);

    /**
     * @brief Destructor de la clase Shader. Libera la memoria del shader con glDeleteProgram().
     */
    ~Shader();

    /**
     * @brief Funcion para activar (bind) el shader.
     */
    void Bind() const;

    /**
     * @brief Funcion para desactivar (unbind) el shader.
     */
    void Unbind() const;

    /**
     * @brief Funcion para crear variables uniformes
     * @param nombreUniforme Nombre de la variable uniforme
     * @param v0 Primer valor de la variable uniforme
     * @param v1 Segundo valor de la variable uniforme
     * @param v2 Tercer valor de la variable uniforme
     * @param v3 Cuarto valor de la variable uniforme
     * @return Reserva un espacio en memoria para la variable uniforme
     */
    void EstableceUniforme4f(const std::string& nombreUniforme, float v0, float v1, float v2, float v3);
private:
    /**
     * @brief Funcion para leer el codigo fuente de un archivo .shader.
     * @param rutaArchivo Ruta relativa al ejecutable de este archivo (src/).
     * @return CodigoFuenteShader Estructura con el codigo fuente del vertex y fragment shaders.
     */
    CodigoFuenteShader ProcesaShader(const std::string& rutaArchivo);

    /**
     * @brief Funcion para compilar un shader en el framework de OpenGL
     * @param tipoShader Tipo de shader a compilar (vertex, fragment, etc)
     * @param codigoFuente Codigo fuente del shader
     * @return id del shader (>0), si ocurre algun error lo muestra en consola
     * y retorna 0.
     */
    unsigned int CompilaShader(unsigned int tipoShader, const std::string& codigoFuente);
    
    /**
     * @brief Funcion para crear un programa en el framework de OpenGL con un 
     * vertex shader y un fragment shader
     * @param vertexShader Codigo fuente del vertex shader
     * @param fragmentShader Codigo fuente del fragment shader
     * @return id del programa (>0), si ocurre algun error lo muestra en consola
     * y retorna 0.
     */
    unsigned int CreaShader(const std::string& vertexShader, const std::string& fragmentShader);
    
    /**
     * @brief Funcion para obtener el indice (locacion) de una variable uniforme
     * @param nombreUniforme Nombre de la variable uniforme
     */
    int ObtenDireccionUniforme(const std::string& nombreUniforme);
};
