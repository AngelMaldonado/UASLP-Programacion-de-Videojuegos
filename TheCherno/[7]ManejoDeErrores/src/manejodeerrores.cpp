/**
 * @brief El manejo de errores es importante para el correcto funcionamiento de la aplicación.
 * OpenGL tiene funciones que ayudan al manejo de errores mediante banderas:
 * > glGetError()
 * > glDebugMessageCallback() <- mejorado desde OpenGL 4.3 +informacion
 * 
 * @author @AngelMaldonado
 * @file bufferdeindices.cpp
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
    /* Crea el buffer para el shader */
    unsigned int id = glCreateShader(tipoShader);
    /* Obten el apuntador del buffer del codigo fuente */
    const char* codigoFuentePtr = codigoFuente.c_str();
    /* Envia el codigo fuente al buffer del shader */
    GLCall(glShaderSource(id, 1, &codigoFuentePtr, nullptr));
    /* Compila el shader */
    GLCall(glCompileShader(id));

    /* Verifica que el shader se haya compilado correctamente */
    int resultadoCompilacion;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &resultadoCompilacion));
    if (resultadoCompilacion == GL_FALSE)
    {
        /* Obten el log de errores */
        /* Longitud del log de errores */
        int longitudLog;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &longitudLog));
        
        /* Crear un buffer para almacenar el log de errores */
        char* log = (char *)alloca(longitudLog * sizeof(char));

        /* Obten el log de errores */
        GLCall(glGetShaderInfoLog(id, longitudLog, &longitudLog, log));

        /* Muestra el log de errores */
        std::cout << "Error de compilacion " << (tipoShader == GL_VERTEX_SHADER ? "vertex" : "fragment");
        std::cout << " shader: " << std::endl;
        std::cout << log << std::endl;

        /* Libera el buffer del programa */
        GLCall(glDeleteShader(id));

        /* Retorna error (0) */
        return 0;
    }

    /* Regresa el id del programa */
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
    /* Crear el buffer para el programa principal */
    unsigned int programa = glCreateProgram();
    /* Crear el buffer para el vertex shader */
    unsigned int vs = CompilaShader(GL_VERTEX_SHADER, vertexShader);
    /* Crear el buffer para el fragment shader */
    unsigned int fs = CompilaShader(GL_FRAGMENT_SHADER, fragmentShader);

    /* Enlaza el vertex y fragment shaders al programa principal */
    GLCall(glAttachShader(programa, vs));
    GLCall(glAttachShader(programa, fs));
    /* Enlaza el programa principal con OpenGL */
    GLCall(glLinkProgram(programa));
    /* Valida el programa principal */
    GLCall(glValidateProgram(programa));

    /* Elimina el vs y fs ya que no se utilizaran mas */
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    /* Regresa el id del programa */
    return programa;
}

int main() {
    GLFWwindow* ventana;
    
    /* Inicializar la ventana */
    if(!glfwInit())
        return -1;

    
    /* Crear la ventana y el contexto en OpenGL */
    ventana = glfwCreateWindow(640, 480, "Ventana", NULL, NULL);
    /* Si no se pudo crear la ventana */
    if(!ventana)
    {
        GLCall(glfwTerminate());
        return -1;
    }

    /* Hacer ventana el contexto actual */
    glfwMakeContextCurrent(ventana);

    /* Inicializar glew despues de haber creado un contexto */
    if(glewInit() != GLEW_OK)
        return -1;

    /* Mostrar informacion de la version de OpenGL */
    std::cout << "Version de OpenGL: " << glGetString(GL_VERSION) << std::endl;

    /**
     * @brief Primeramente se define el buffer con los datos (bytes) que se le
     * pasaran a OpenGL.
     */
    /* Buffer con las posiciones de un triangulo */
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
    /* Crear el buffer (1) */
    GLCall(glGenBuffers(1, &vbo));
    /* Selecciona el buffer recien credo como el buffer actual */
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
        /* Una vez seleccionado, se puede manipular los datos dentro del buffer */
        // STATIC_DRAW: los datos no cambian pero si se muestran
        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 2, posiciones, GL_STATIC_DRAW));

        /**
         * @brief Una vez definido el buffer con los datos, y OpenGL ya tiene conocimiento
         * de este buffer, se debe especificar lo que significa cada dato dentro del buffer.
         * Una manera de hacer esto es especificando la distribucion de los datos mediante
         * indices, donde cada indice representara un atributo de los vertices.
         * La funcion glVertexAttribPointer le especifica a OpenGL la distribucion del buffer
         * y recibe como parametros:
         * - El indice del atributo (0, 1, 2, 3, 4, 5, etc)
         * - El numero de componentes de cada atributo (2, 3, 4, etc)
         * - El tipo de dato de cada componente (GL_FLOAT, GL_INT, etc)
         * - Si el dato es normalizado (GL_TRUE, GL_FALSE) -> datos decimales entre 0 y 1
         * - El offset (en bytes) entre cada conjunto de atributos de vertices [0, 6, 12, etc]
         * - El apuntador al buffer con los datos
         */
        /* Habilitar el atributo */
        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
    
    /* IBO: Index Buffer Object (arreglo con los indices de las coordenadas que forman cada triangulo) */
    unsigned int ibo;
    /* Crear el buffer (1) */
    GLCall(glGenBuffers(1, &ibo));
    /* Selecciona el buffer recien credo como el buffer actual */
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo)); // <- se define como ELEMENT_ARRAY_BUFFER
        /* Una vez seleccionado, se puede manipular los datos dentro del buffer */
        // STATIC_DRAW: los datos no cambian pero si se muestran
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indices, GL_STATIC_DRAW));
        /* El IBO no necesita habilitarse como atributo */

    /* Procesa el archivo .shader */
    CodigoFuenteShader codigoShader = ProcesaShader("../res/shaders/Basico.shader");
    /* Crea y compila el programa principal de los shaders */
    unsigned int shader = CrearShader(codigoShader.CodigoVertex, codigoShader.CodigoFragment);
    /* Selecciona el programa principal */
    GLCall(glUseProgram(shader));

    /* Bucle principal (hasta cerrar ventana) */
    while(!glfwWindowShouldClose(ventana))
    {
        /* Proceso de renderizado */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        /* Como ahora tenemos un buffer con los indices a los vertices se cambia por la funcion glDrawElements */
        /* (Modo, 6 indices, tipo de dato de los indices, apuntador al arreglo de indices) */
        /* Debido a que se hizo bind al arreglo de indices no es necesario pasar por parametro el arreglo de indices */
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

        /* Cambia los buffers delantero y trasero */
        GLCall(glfwSwapBuffers(ventana));

        /* Proceso de eventos */
        GLCall(glfwPollEvents());
    }

    /* Limpia el buffer del programa del shader */
    glDeleteProgram(shader);
    /* Cerrar ventana */
    glfwDestroyWindow(ventana);

    return 0;
}
