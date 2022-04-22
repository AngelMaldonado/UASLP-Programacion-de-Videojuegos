/**
 * @brief Abstraer OpenGL en clases es importante para la optimizacion de codigo,
 * ya que reduce bastante la cantidad de lineas de codigo y facilita su lectura.
 * 
 * @author @AngelMaldonado
 * @file openglenclases.cpp
 * @link glfw: https://www.glfw.org/ (3.3)
 * @link glew: https://glew.sourceforge.io/ (7.0)
 * @version 1.0 Fri Apr 22 2022
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

/* Inclusion de las librerias con las nuevas clases */
#include "Renderizador.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

struct CodigoFuenteShader {
    std::string CodigoVertex;
    std::string CodigoFragment;
};

/**
 * @brief Funcion para leer el codigo fuente de un archivo .shader.
 * @param rutaArchivo Ruta relativa al ejecutable de este archivo (src/).
 * @return CodigoFuenteShader Estructura con el codigo fuente del vertex y fragment shaders.
 */
static CodigoFuenteShader ProcesaShader(const std::string& rutaArchivo)
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

/**
 * @brief Funcion para compilar un shader en el framework de OpenGL
 * @param tipoShader Tipo de shader a compilar (vertex, fragment, etc)
 * @param codigoFuente Codigo fuente del shader
 * @return id del shader (>0), si ocurre algun error lo muestra en consola
 * y retorna 0.
 */
static unsigned int CompilaShader(unsigned int tipoShader, const std::string& codigoFuente)
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

/**
 * @brief Funcion para crear un programa en el framework de OpenGL con un 
 * vertex shader y un fragment shader
 * @param vertexShader Codigo fuente del vertex shader
 * @param fragmentShader Codigo fuente del fragment shader
 * @return id del programa (>0), si ocurre algun error lo muestra en consola
 * y retorna 0.
 */
static int CrearShader(const std::string& vertexShader, const std::string& fragmentShader)
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

int main() {
    GLFWwindow* ventana;
    
    if(!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    ventana = glfwCreateWindow(640, 480, "Ventana", NULL, NULL);
    if(!ventana)
    {
        GLCall(glfwTerminate());
        return -1;
    }

    glfwMakeContextCurrent(ventana);

    glfwSwapInterval(1);

    if(glewInit() != GLEW_OK)
        return -1;

    std::cout << "Version de OpenGL: " << glGetString(GL_VERSION) << std::endl;
    /* Englobar el codigo donde se llama GLCall en un scope para poder cerrar la aplicacion */
    {
        float posiciones[] {
        -0.5f, -0.5f,
            0.5f, -0.5f,
            0.5f,  0.5f,
        -0.5f,  0.5f,
        };

        unsigned int indices[] {
            0, 1, 2,
            2, 3, 0 
        };

        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

            /* Creacion del objeto vbo con la clase VertexBuffer */
            VertexBuffer vbo(posiciones, 4 * 2 * sizeof(float));
            GLCall(glEnableVertexAttribArray(0));
            GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

        /* Creacion del objeto ibo con la clase IndexBuffer */
        IndexBuffer ibo(indices, 6);

        CodigoFuenteShader codigoShader = ProcesaShader("../res/shaders/Basico.shader");
        unsigned int shader = CrearShader(codigoShader.CodigoVertex, codigoShader.CodigoFragment);
        GLCall(glUseProgram(shader));

        GLCall(int dir_u_Color = glGetUniformLocation(shader, "u_Color"));
        ASSERT(dir_u_Color != -1);
        GLCall(glUniform4f(dir_u_Color, 0.8f, 0.3f, 0.8f, 1.0f));

        GLCall(glBindVertexArray(0));
        GLCall(glUseProgram(0));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));


        float r = 0.0f;
        float incr = 0.05f;
        while(!glfwWindowShouldClose(ventana))
        {

            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            GLCall(glUseProgram(shader));
            GLCall(glUniform4f(dir_u_Color, r, 0.3f, 0.8f, 1.0f));

            /* No hace falta hacer bind del vbo ya que se definio como atributo dentro del vao */
            GLCall(glBindVertexArray(vao));
            
            /* Bind del ibo */
            ibo.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

            if(r > 1.0f)
                incr = -0.05f;
            else if(r < 0.0f)
                incr = 0.05f;
            r += incr;

            GLCall(glfwSwapBuffers(ventana));

            GLCall(glfwPollEvents());
        }

        GLCall(glDeleteProgram(shader));
    }
    glfwDestroyWindow(ventana);

    return 0;
}
