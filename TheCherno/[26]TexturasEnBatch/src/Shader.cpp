/**
 * @brief A new C++ implementation file
 * 
 * @author @AngelMaldonado
 * @file Shader.cpp
 * @link www.somelink.com
 * @version 1.0 Fri Apr 22 2022
 */

#include <iostream>
#include <fstream>
#include <sstream>

#include "Shader.h"
#include "Renderizador.h"

Shader::Shader(const std::string& rutaArchivoShader) 
    : m_rutaArchivoShader(rutaArchivoShader), m_IDRenderizador(0)
{
    CodigoFuenteShader codigoShader = ProcesaShader(rutaArchivoShader);
    m_IDRenderizador = CreaShader(codigoShader.CodigoVertex, codigoShader.CodigoFragment);
}

Shader::~Shader() 
{
    GLCall(glDeleteProgram(m_IDRenderizador));    
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_IDRenderizador));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::EstableceUniforme1i(const std::string& nombreUniforme, int v0) 
{
    GLCall(glUniform1i(ObtenDireccionUniforme(nombreUniforme), v0));
}

void Shader::EstableceUniforme1iv(const std::string& nombreUniforme, const std::vector<int>& valores) 
{
    GLCall(glUniform1iv(ObtenDireccionUniforme(nombreUniforme), valores.size(), &valores[0]));
}

void Shader::EstableceUniforme4f(const std::string& nombreUniforme, float v0, float v1, float v2, float v3) 
{
    GLCall(glUniform4f(ObtenDireccionUniforme(nombreUniforme), v0, v1, v2, v3));
}

void Shader::EstableceUniformeMat4f(const std::string& nombreUniforme, const glm::mat4& matriz) 
{
    GLCall(glUniformMatrix4fv(ObtenDireccionUniforme(nombreUniforme), 1, GL_FALSE, &matriz[0][0]));
}

int Shader::ObtenDireccionUniforme(const std::string& nombreUniforme) 
{
    if (m_cacheDireccionesUniformes.find(nombreUniforme) != m_cacheDireccionesUniformes.end())
        return m_cacheDireccionesUniformes[nombreUniforme];

    GLCall(int direccionUniforme = glGetUniformLocation(m_IDRenderizador, nombreUniforme.c_str()));
    if(direccionUniforme == -1) 
        std::cout << "No se encontro la direccion de la variable uniforme: " << nombreUniforme << std::endl;
        
    m_cacheDireccionesUniformes[nombreUniforme] = direccionUniforme;
    return direccionUniforme;
}

unsigned int Shader::CreaShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int programa = glCreateProgram();
    unsigned int vs = CompilaShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompilaShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(programa, vs));
    GLCall(glAttachShader(programa, fs));
    GLCall(glLinkProgram(programa));
    GLCall(glValidateProgram(programa));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return programa;
}

unsigned int Shader::CompilaShader(unsigned int tipoShader, const std::string& codigoFuente)
{
    GLCall(unsigned int id = glCreateShader(tipoShader));
    const char* codigoFuentePtr = codigoFuente.c_str();
    
    GLCall(glShaderSource(id, 1, &codigoFuentePtr, nullptr));
    GLCall(glCompileShader(id));

    int resultadoCompilacion;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &resultadoCompilacion));

    if (resultadoCompilacion == GL_FALSE)
    {
        int longitudLog;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &longitudLog));
        
        char* log = (char *)alloca(longitudLog * sizeof(char));


        GLCall(glGetShaderInfoLog(id, longitudLog, &longitudLog, log));

        std::cout << "Error de compilacion " << (tipoShader == GL_VERTEX_SHADER ? "vertex" : "fragment");
        std::cout << " shader: " << std::endl;
        std::cout << log << std::endl;


        GLCall(glDeleteShader(id));

        return 0;
    }

    return id;
}

CodigoFuenteShader Shader::ProcesaShader(const std::string& rutaArchivo)
{
    std::ifstream bytesArchivo(rutaArchivo);

    enum class TipoShader
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string linea;
    std::stringstream contenido[2];
    TipoShader tipoActual = TipoShader::NONE;
    while(getline(bytesArchivo, linea))
    {
        if(linea.find("#shader") != std::string::npos)
        {
            if(linea.find("vertex") != std::string::npos)
                tipoActual = TipoShader::VERTEX;
            else if (linea.find("fragment") != std::string::npos)
                tipoActual = TipoShader::FRAGMENT;
        }
        else
        {
            contenido[(int)tipoActual] << linea << '\n';
        }
    }

    return { contenido[0].str(), contenido[1].str() };
}
