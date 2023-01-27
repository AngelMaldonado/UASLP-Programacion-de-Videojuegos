/**
 * @brief El manejo de errores es importante para el correcto funcionamiento de la aplicación.
 * OpenGL tiene funciones que ayudan al manejo de errores mediante banderas:
 * > glGetError()
 * > glDebugMessageCallback() <- mejorado desde OpenGL 4.3 +informacion
 * 
 * @author @AngelMaldonado
 * @file manejodeerrores.cpp
 * @link glfw: https://www.glfw.org/ (3.3)
 * @link glew: https://glew.sourceforge.io/ (7.0)
 * @version 1.0 Thu Apr 21 2022
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

/* Asercion para errores */
#define ASSERT(x) if(!(x)) __debugbreak();
/* Llamada a ambas funciones */
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

/**
 * @brief Funcion para limpiar el buffer de errores
 */
static void GLClearError()
{
    /* Mientras siga habiendo errores por mostrar */
    while(glGetError() != GL_NO_ERROR);
}

/**
 * @brief Funcion para mostrar los errores de OpenGL
 * @param funcion Funcion donde se evaluan los errores
 * @param archivo Archivo donde se encuentra la funcion
 * @param linea Linea donde se encuentra la funcion
 */
static bool GLLogCall(const char* funcion, const char* archivo, int linea)
{
    // Mientras se pueda obtener errores
    while(GLenum error = glGetError())
    {
        // Mostrar el error
        std::cout << "[OpenGL Error] ( " << error << " ): " << funcion <<
            " " << archivo << ":" << linea << std::endl;
        return false;
    }
    return true;
}

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
    unsigned int id = glCreateShader(tipoShader);
    const char* codigoFuentePtr = codigoFuente.c_str();
    
    /** Llamada GLCall() **/
    GLCall(glShaderSource(id, 1, &codigoFuentePtr, nullptr));
    /** Llamada GLCall() **/
    GLCall(glCompileShader(id));

    int resultadoCompilacion;
    /** Llamada GLCall() **/
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &resultadoCompilacion));

    if (resultadoCompilacion == GL_FALSE)
    {
        int longitudLog;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &longitudLog));
        
        char* log = (char *)alloca(longitudLog * sizeof(char));

        /** Llamada GLCall() **/
        GLCall(glGetShaderInfoLog(id, longitudLog, &longitudLog, log));

        std::cout << "Error de compilacion " << (tipoShader == GL_VERTEX_SHADER ? "vertex" : "fragment");
        std::cout << " shader: " << std::endl;
        std::cout << log << std::endl;

        /** Llamada GLCall() **/
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

    /** Llamadas GLCall() **/
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

    
    ventana = glfwCreateWindow(640, 480, "Ventana", NULL, NULL);
    if(!ventana)
    {
        GLCall(glfwTerminate());
        return -1;
    }

    glfwMakeContextCurrent(ventana);

    if(glewInit() != GLEW_OK)
        return -1;

    std::cout << "Version de OpenGL: " << glGetString(GL_VERSION) << std::endl;

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

    unsigned int vbo;
    /** Llamadas GLCall() **/
    GLCall(glGenBuffers(1, &vbo));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 2, posiciones, GL_STATIC_DRAW));

        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
    
    unsigned int ibo;
    /** Llamadas GLCall() **/
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo)); // <- se define como ELEMENT_ARRAY_BUFFER
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indices, GL_STATIC_DRAW));

    CodigoFuenteShader codigoShader = ProcesaShader("../res/shaders/Basico.shader");
    unsigned int shader = CrearShader(codigoShader.CodigoVertex, codigoShader.CodigoFragment);
    /** Llamada GLCall() **/
    GLCall(glUseProgram(shader));

    while(!glfwWindowShouldClose(ventana))
    {
        /** Llamadas GLCall() **/
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        /* Se puede probar GLCall cambiando GL_UNSIGNED_INT por GL_INT */
        // GLCall(glDrawElements(GL_TRIANGLES, 6, GL_INT, 0));
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

        GLCall(glfwSwapBuffers(ventana));

        GLCall(glfwPollEvents());
    }

    GLCall(glDeleteProgram(shader));
    glfwDestroyWindow(ventana);

    return 0;
}
