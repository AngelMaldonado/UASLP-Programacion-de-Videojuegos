/**
 * @brief El arreglo de vertices (vertex array) es una caracteristica de OpenGL,
 * es un arreglo creado para almacenar buffers de datos que especifica cada uno
 * los atributos de los vertices dependiendo el objeto, cada posicion dentro del
 * arreglo vao puede indicar un atributo diferente para un mismo objeto, o
 * diferentes atributos para diferentes objetos.
 * 
 * El vao ya esta creado por defecto en modo de COMPATIBILIDAD de OpenGL, 
 * por eso se hace bind en 0, y el modo CORE no crea el vao. En este programa
 * se cambia a modo CORE para que se pueda crear el vao manualmente.
 * Entonces, en modo de COMPATIBILIDAD se puede utilizar el VAO por defecto
 * y especificar la distribucion del vbo constantemente o crear el VAO manualmente,
 * mientras que en modo CORE se puede hacer lo mismo pero siempre se debera crear el
 * VAO manualmente.
 * 
 * Esto deja la conclusion de que puede haber 2 tipos de VAOs:
 * - VAO global
 * - Multiples VAOs
 * 
 * El vao cambia la manera en la que se prepara el objeto para dibujar de:
 * 
 * 1. Bind shader
 * 2. Bind buffer de vertices
 * 3. Establecer la distribucion del buffer
 * 4. Bind buffer de indices
 * 
 * a simplemente:
 * 
 * 1. Bind shader
 * 2. Bind objeto de arreglo de vertices (vao)
 * 3. Bind buffer de indices
 * 
 * Esto quiere decir que el vertex array object tiene una relacion directa (bind) con
 * el buffer de vertices y la distribucion de los atributos de los vertices.
 * 
 * @author @AngelMaldonado
 * @file arreglosdevertices.cpp
 * @link glfw: https://www.glfw.org/ (3.3)
 * @link glew: https://glew.sourceforge.io/ (7.0)
 * @version 1.0 Thu Apr 21 2022
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

/**
 * @brief Funcion para limpiar el buffer de errores
 */
static void GLClearError()
{
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
    while(GLenum error = glGetError())
    {
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

    /* Modificar la version de OpenGL y el modo de compatibilidad */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // version 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // perfil CORE

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

    /**
     * @brief Si tuvieramos varios objetos, se tendria que hacer bind y unbind de los buffers
     * vao, vbo, ibo y los shaders para cada objeto constantemente.
     */
    /* Creacion del VAO (Vertex Array Object) */
    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

        /* Todos los buffers que se enlacen con glVertexAttribPointer quedaran dentro del vao actual */
        unsigned int vbo;
        GLCall(glGenBuffers(1, &vbo));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
            /* Como se hizo bind al vbo, la distribucion se hara en este buffer */
            GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 2, posiciones, GL_STATIC_DRAW));

            /* El perfil CORE arrojaria un error en esta funcion porque aun no crea el VAO */
            GLCall(glEnableVertexAttribArray(0));
            /* Esta es la instruccion que enlaza el buffer de los vertices (vbo) al vao, posicion 0 dentro del vao */
            GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
        
    unsigned int ibo;
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indices, GL_STATIC_DRAW));

    CodigoFuenteShader codigoShader = ProcesaShader("../res/shaders/Basico.shader");
    unsigned int shader = CrearShader(codigoShader.CodigoVertex, codigoShader.CodigoFragment);
    GLCall(glUseProgram(shader));

    GLCall(int dir_u_Color = glGetUniformLocation(shader, "u_Color"));
    ASSERT(dir_u_Color != -1);
    GLCall(glUniform4f(dir_u_Color, 0.8f, 0.3f, 0.8f, 1.0f));

    /* Unbind de los buffers */
    GLCall(glBindVertexArray(0));
    GLCall(glUseProgram(0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));


    float r = 0.0f;
    float incr = 0.05f;
    while(!glfwWindowShouldClose(ventana))
    {

        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        /** 
         * Si ejecutamos glUniform4f sin hacer bind a los buffers, marcara
         * GL_INVALID_OPERATION, porque no encuentre un programa de shader en
         * el buffer de shaders, lo mismo para glDrawElements.
         */
        GLCall(glUseProgram(shader));
        GLCall(glUniform4f(dir_u_Color, r, 0.3f, 0.8f, 1.0f));

        /** 
         * La accion de establecer la distribucion especifica dentro del vertex array object (vao)
         * le da al indice especifico dentro del vao dicha distribucion con la informacion especificada.
         * Por lo tanto ya no haria falta habilitar el atributo, especificar la distribucion, ni hacer bind
         * al buffer, solo con hacer bind al vao seria suficiente.
         */
        // GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
        // GLCall(glEnableVertexAttribArray(0));
        // GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
        GLCall(glBindVertexArray(vao));

        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
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
    glfwDestroyWindow(ventana);

    return 0;
}
