/**
 * @brief Las primitivas de las GPUs son los triangulos, por lo que todas las
 * operaciones de renderizado se realizan con un minimo de 3 vertices para poder
 * dibujar un plano en la pantalla, por lo tanto si se intenta dibujar un rectangulo,
 * se interpretara como 2 triangulos. Y los buffers de indices sirven principalmente para
 * evitar la repeticion de vertices en el buffer de vertices, y simplemente hacer referencia a
 * los vertices que ya existen mediante indices.
 * 
 * @author @AngelMaldonado
 * @file bufferdeindices.cpp
 * @link glfw: https://www.glfw.org/ (3.3)
 * @link glew: https://glew.sourceforge.io/ (7.0)
 * @version 1.0 Tue Apr 19 2022
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

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

    float posiciones[] {
       -0.5f, -0.5f, // (0) abajo-izquierda
        0.5f, -0.5f, // (1) abajo-derecha
        0.5f,  0.5f, // (2) arriba-derecha
       -0.5f,  0.5f, // (3) arriba-izquierda
    };

    /* Buffer con los indices de los vertices para cada triangulo */
    unsigned int indices[] {
        0, 1, 2, // triangulo 1
        2, 3, 0  // triangulo 2
    };

    /* VBO: Vertex Buffer Object (arreglo con las coordenadas de los vertices) */
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 2, posiciones, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    
    /* IBO: Index Buffer Object (arreglo con los indices de las coordenadas que forman cada triangulo) */
    unsigned int ibo;
    /* Crear el buffer (1) */
    glGenBuffers(1, &ibo);
    /* Selecciona el buffer recien credo como el buffer actual */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); // <- se define como ELEMENT_ARRAY_BUFFER
        /* Una vez seleccionado, se puede manipular los datos dentro del buffer */
        // STATIC_DRAW: los datos no cambian pero si se muestran
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indices, GL_STATIC_DRAW);
        /* El IBO no necesita habilitarse como atributo */

    CodigoFuenteShader codigoShader = ProcesaShader("../res/shaders/Basico.shader");
    unsigned int shader = CrearShader(codigoShader.CodigoVertex, codigoShader.CodigoFragment);
    glUseProgram(shader);

    while(!glfwWindowShouldClose(ventana))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        /* Como ahora tenemos un buffer con los indices a los vertices se cambia por la funcion glDrawElements */
        /* (Modo, 6 indices, tipo de dato de los indices, apuntador al arreglo de indices) */
        /* Debido a que se hizo bind al arreglo de indices no es necesario pasar por parametro el arreglo de indices */
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(ventana);

        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwDestroyWindow(ventana);

    return 0;
}
