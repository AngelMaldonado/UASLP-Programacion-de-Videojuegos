/**
 * @brief Un shader es un programa que se ejecuta en la GPU por cada vertice
 * que exista dentro del buffer de datos en OpenGL, la principal razon por
 * la que se utilizan los shaders es porque libera trabajo a la CPU para dejar
 * a la GPU encargada de todas las tareas relacionadas con el renderizado y las
 * tareas graficas.
 * Los shaders practicamente le dicen a la GPU como debe renderizar un objeto;
 * que colores debe utilizar, que texturas debe utilizar, que posiciones, etc.
 * Hay varios tipos de shaders:
 * - Vertex Shader: Este es el encargado de dibujar los vertices de un objeto.
 * - Fragment Shader: Este es el encargado de dibujar los fragmentos de un objeto,
 *   los fragmentos pueden ser considerados *pixeles* (proceso de rasterizacion -
 *   color), por lo tanto el Fragment Shader se ejecuta por cada pixel del objeto.
 * ... y muchos mas.
 * Los shaders pueden enviar informacion entre ellos y utilizarla.
 * 
 * @author @AngelMaldonado
 * @file shaders.cpp
 * @link glfw: https://www.glfw.org/ (3.3)
 * @link glew: https://glew.sourceforge.io/ (7.0)
 * @version 1.0 Tue Apr 19 2022
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

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
    glShaderSource(id, 1, &codigoFuentePtr, nullptr);
    /* Compila el shader */
    glCompileShader(id);

    /* Verifica que el shader se haya compilado correctamente */
    int resultadoCompilacion;
    glGetShaderiv(id, GL_COMPILE_STATUS, &resultadoCompilacion);
    if (resultadoCompilacion == GL_FALSE)
    {
        /* Obten el log de errores */
        /* Longitud del log de errores */
        int longitudLog;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &longitudLog);
        
        /* Crear un buffer para almacenar el log de errores */
        char* log = (char *)alloca(longitudLog * sizeof(char));

        /* Obten el log de errores */
        glGetShaderInfoLog(id, longitudLog, &longitudLog, log);

        /* Muestra el log de errores */
        std::cout << "Error de compilacion " << (tipoShader == GL_VERTEX_SHADER ? "vertex" : "fragment");
        std::cout << " shader: " << std::endl;
        std::cout << log << std::endl;

        /* Libera el buffer del programa */
        glDeleteShader(id);

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
    glAttachShader(programa, vs);
    glAttachShader(programa, fs);
    /* Enlaza el programa principal con OpenGL */
    glLinkProgram(programa);
    /* Valida el programa principal */
    glValidateProgram(programa);

    /* Elimina el vs y fs ya que no se utilizaran mas */
    glDeleteShader(vs);
    glDeleteShader(fs);

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
        glfwTerminate();
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
    float posiciones[6] {
       -0.5f, -0.5f,
        0.0f,  0.5f,
        0.5f, -0.5f
    };
    /* ID del buffer, con este ID OpenGL sabra localizar el buffer en cuestion */
    unsigned int bufferID;
    /* Crear el buffer (1) */
    glGenBuffers(1, &bufferID);
    /* Selecciona el buffer recien credo como el buffer actual */
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
        /* Una vez seleccionado, se puede manipular los datos dentro del buffer */
        // STATIC_DRAW: los datos no cambian pero si se muestran
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, posiciones, GL_STATIC_DRAW);

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
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

        /* Define el vertex shader */
        std::string vertexShader =
            "#version 330 core\n"
            "\n"
            /* Acceso a la distribucion de los atributos (location = 0) <- indice del atributo */
            /* in <- variable de entrada */
            /* vec4 ya que la funcion gl_Position requiere un vec4 */
            "layout (location = 0) in vec4 posicion;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    gl_Position = posicion;\n"   
            "}\n";
        
        /* Define el fragment shader */
        std::string fragmentShader =
            "#version 330 core\n"
            "\n"
            "layout (location = 0) out vec4 color;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    color = vec4(1.0, 0.0, 0.0, 1.0);\n"
            "}\n";
        
        /* Crea y compila el programa principal de los shaders */
        unsigned int shader = CrearShader(vertexShader, fragmentShader);
        /* Selecciona el programa principal */
        glUseProgram(shader);

    /* Bucle principal (hasta cerrar ventana) */
    while(!glfwWindowShouldClose(ventana))
    {
        /* Proceso de renderizado */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Cambia los buffers delantero y trasero */
        glfwSwapBuffers(ventana);

        /* Proceso de eventos */
        glfwPollEvents();
    }

    /* Limpia el buffer del programa del shader */
    glDeleteProgram(shader);
    /* Cerrar ventana */
    glfwDestroyWindow(ventana);

    return 0;
}
