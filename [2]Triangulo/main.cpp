/******************************************
*  Autor : Angel de Jesus Maldonado Juarez   
*  Creado el : Vie Feb 11 2022
*  Archivo : main.cpp
*  Descripcion : Primer programa con glfw
*******************************************/
// Bibliotecas I/O
#include <stdio.h>
// Biblioteca de compatibilidad
#include <GL/glew.h>
// API de OpenGL
#include <GLFW/glfw3.h>

// Bibliotecas para manejo de matrices gráficas
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Codigo para cargar shaders como programas
#include "shader.hpp"

using namespace glm;

// Referencia global a la ventana
GLFWwindow* ventana;

// Matrices de transformación
mat4 modelo, vista, proyeccion;

// Funcion para inicializar la ventana con GLFW, glew y OpenGL
int InicializaVentana();

void genMatrices();

// Main
int main(void)
{
    // Si se pudo inicializar la ventana
    if(InicializaVentana() == 0)
    {
        printf("Error al inicializar la ventana\n");
        return -1;
    }

    // Vertices del trinagulo
    // Propiedades de los vertices:
    // 1. Posicion
    // 2. Color
    // 3. Normal
    // 4. Textura
    // 5. Coordenada de la textura
    // 6. ...
    // Las propiedades se pasan al vs directamente en forma de arreglo
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    // Buffer de objetos de vertices (Guarda la informacion)
    unsigned int VBO;
    // Objeto de arreglo de indices de vertices (Guarda la configuración de las propiedades de los vertices)
    unsigned int VAO;

    // Crear el buffer de memoria para los vertices
    glGenBuffers(1, &VBO);
    // Crear el buffer de memoria para la configuracion de los vertices
    glGenVertexArrays(1, &VAO);


    // Asignar el buffer de memoria para los indices
    glBindVertexArray(VAO);
    // Asignar el buffer de memoria para la configuracion de los vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Asignar los datos del buffer de memoria para los vertices
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Asignar los datos del buffer de memoria para los indices
    // indice del vertice, 
    // numero de atributos, 
    // tipo de dato, 
    // normalizado, 
    // stride/bytes para el siguiente vertice, 
    // offset/bytes desde el inicio del buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    // *** Esta configuracion (VAO) solo funcionara para VBO

    // Habilitar el buffer de memoria para los vertices
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Se crean 2 programas de shaders (vertex y fragment)
    GLuint programaID = LoadShaders("C:\\Users\\amald\\OneDrive - Universidad Autonoma de San Luis Potosi - UASLP\\Programacion\\Programacion de Videojuegos\\[2]Triangulo\\vs1.glsl",
                                    "C:\\Users\\amald\\OneDrive - Universidad Autonoma de San Luis Potosi - UASLP\\Programacion\\Programacion de Videojuegos\\[2]Triangulo\\fs1.glsl");

    // Ciclo principal
    do
    {
        // Limpia la ventana de color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Seleccionar el programa de shaders
        glUseProgram(programaID);

        // Variables uniformes
        // Obten la direccion de "colorUniform"
        int idUniform = glGetUniformLocation(programaID, "colorUniform");
        // 3f -> va a meter 3 flotantes en la direccion de "colorUniform"
        glUniform3f(idUniform, 1.0f, 1.0f, 0.0f);


        int idFactorAmb = glGetUniformLocation(programaID, "factorAmbiental");
        glUniform1f(idFactorAmb, 0.5f);


        // Matrices de transformacion
        genMatrices();
        int modeloDir = glGetUniformLocation(programaID, "modelo");
        glUniformMatrix4fv(modeloDir, 1, GL_FALSE, value_ptr(modelo));
        modeloDir = glGetUniformLocation(programaID, "vista");
        glUniformMatrix4fv(modeloDir, 1, GL_FALSE, value_ptr(vista));
        modeloDir = glGetUniformLocation(programaID, "proyeccion");
        glUniformMatrix4fv(modeloDir, 1, GL_FALSE, value_ptr(proyeccion));

        glBindVertexArray(VAO);
        // Dibujar el triangulo
        glDrawArrays(GL_TRIANGLES, 0, 3);// Primer indice, cantidad de vertices

        // Actualizar la ventana (buffer)
        glfwSwapBuffers(ventana);
        // Esperar una actualizacion
        glfwPollEvents();
    }while(glfwGetKey(ventana, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(ventana) == 0);
    // Liberar los recursos
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(programaID);
    // Cerrar la ventana
    glfwTerminate();
    return 0;
}

void genMatrices() 
{
    // Modelo
    modelo = mat4(1);
    modelo = translate(modelo, vec3(0.5f, 0.0f, 0.0f));
    
    // Vista
    vec3 ojo(0.0f, 1.0f, 5.0f);
    vec3 centro(0.0f, 0.0f, 0.0f);
    vec3 up(0.0f, 1.0f, 0.0f);
    vista = lookAt(ojo, centro, up);

    // Proyeccion
    proyeccion = perspective(radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

}

int InicializaVentana()
{
    // Inicializar GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Fallo al inicializar GLFW\n");
        return -1;
    }

    // Caracteristicas de la ventana (definir versioens de OpenGL)
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    
    // Crear la ventana
    ventana = glfwCreateWindow(800, 600, "GLFW | Primer programa", NULL, NULL);
    // El contexto de OpenGL se establece en el hilo de la ventana
    // , todas las operaciones de OpenGL se ejecutan sobre ventana
    glfwMakeContextCurrent(ventana);
    // Establece la lectura de entradas a ventana
    glfwSetInputMode(ventana, GLFW_STICKY_KEYS, GL_TRUE);
    // Si no se pudo crear la ventana
    if (ventana == NULL)
    {
        fprintf(stderr, "Fallo al crear la ventana\n");
        glfwTerminate();
        return -1;
    }

    // Inicializar GLEW
    glewExperimental = GL_TRUE;
    // Si no se pudo inicializar
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Fallo al inicializar GLEW\n");
        return -1;
    }

    // Inicializar OpenGL
    // Limpia el color de fondo de la ventana
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // Habilita el z-buffer
    glEnable(GL_DEPTH_TEST);

    // La ventana se creo correctamente
    return 1;
}