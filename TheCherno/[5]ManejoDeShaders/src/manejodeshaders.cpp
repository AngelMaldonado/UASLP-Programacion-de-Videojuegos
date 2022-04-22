/**
 * @brief Normalmente los shaders se crean en archivos, y lo mas comun es que
 * el vertex shader y fragment shader se encuentren en archivos distintos, sin embargo
 * se pueden crear en el mismo archivo.
 * 
 * @author @AngelMaldonado
 * @file manejodeshaders.cpp
 * @link glfw: https://www.glfw.org/ (3.3)
 * @link glew: https://glew.sourceforge.io/ (7.0)
 * @version 1.0 Tue Apr 19 2022
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

/* Estructura para almacenar el codigo fuente del vertex y fragment shaders */
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
    /* Lee los bytes del archivo */
    std::ifstream bytesArchivo(rutaArchivo);

    /* Tipo actual del shader en la lectura del archivo */
    enum class TipoShader
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    /* Recorre el archivo linea por linea */
    std::string linea;
    std::stringstream contenido[2];
    TipoShader tipoActual = TipoShader::NONE;
    while(getline(bytesArchivo, linea))
    {
        /* Si es un nuevo tipo de shader */
        if(linea.find("#shader") != std::string::npos)
        {
            /* Si es vertex */
            if(linea.find("vertex") != std::string::npos)
                tipoActual = TipoShader::VERTEX;
            /* Si es fragment */
            else if (linea.find("fragment") != std::string::npos)
                tipoActual = TipoShader::FRAGMENT;
        }
        /* Si es una linea de codigo */
        else
        {
            /* Agrega el codigo al contenido del shader */
            contenido[(int)tipoActual] << linea << '\n';
        }
    }

    /* Regresa una estructura CodigoFuenteShader con los 2 codigos fuentes */
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
    unsigned int id = glCreateShader(tipoShader);
    const char* codigoFuentePtr = codigoFuente.c_str();

    glShaderSource(id, 1, &codigoFuentePtr, nullptr);
    glCompileShader(id);

    int resultadoCompilacion;
    glGetShaderiv(id, GL_COMPILE_STATUS, &resultadoCompilacion);

    if (resultadoCompilacion == GL_FALSE)
    {
        int longitudLog;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &longitudLog);
        
        char* log = (char *)alloca(longitudLog * sizeof(char));

        glGetShaderInfoLog(id, longitudLog, &longitudLog, log);

        std::cout << "Error de compilacion " << (tipoShader == GL_VERTEX_SHADER ? "vertex" : "fragment");
        std::cout << " shader: " << std::endl;
        std::cout << log << std::endl;

        glDeleteShader(id);

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

    glAttachShader(programa, vs);
    glAttachShader(programa, fs);
    glLinkProgram(programa);
    glValidateProgram(programa);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return programa;
}

int main() {
    GLFWwindow* ventana;
    
    if(!glfwInit())
        return -1;

    
    ventana = glfwCreateWindow(640, 480, "Ventana", NULL, NULL);
    if(!ventana)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(ventana);

    if(glewInit() != GLEW_OK)
        return -1;

    std::cout << "Version de OpenGL: " << glGetString(GL_VERSION) << std::endl;

    float posiciones[6] {
       -0.5f, -0.5f,
        0.0f,  0.5f,
        0.5f, -0.5f
    };
    unsigned int bufferID;
    glGenBuffers(1, &bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, posiciones, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    /* Procesa el archivo .shader */
    CodigoFuenteShader codigoShader = ProcesaShader("../res/shaders/Basico.shader");
    /* Crea y compila el programa principal de los shaders */
    unsigned int shader = CrearShader(codigoShader.CodigoVertex, codigoShader.CodigoFragment);
    glUseProgram(shader);

    while(!glfwWindowShouldClose(ventana))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(ventana);

        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwDestroyWindow(ventana);

    return 0;
}
