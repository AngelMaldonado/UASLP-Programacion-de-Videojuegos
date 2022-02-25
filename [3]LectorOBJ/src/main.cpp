/******************************************
*  Autor : Angel Maldonado & Kevin Avila
*  Creado el : Vie Feb 25 2022
*  Archivo : main.cpp
*  Descripcion : Lectura de archivos .obj
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
#include "../include/shader.hpp"
// Lector obj
#include "../include/objreader.h"

// Referencia global a la ventana
GLFWwindow* ventana;

// Funcion para inicializar la ventana con GLFW, glew y OpenGL
int InicializaVentana();

// Main
int main(void)
{
    // Si se pudo inicializar la ventana
    if(InicializaVentana() == 0)
    {
        printf("Error al inicializar la ventana\n");
        return -1;
    }

    // Lectura del objeto
    Object obj;
    OBJFileReader objReader;
    obj = objReader.readFile("../obj/test/cube.obj", 'n');

    // Extraccion de los vertices como vectores
    vector<Vertex> objVertices;
    for(Mesh mesh : obj.getMeshes())
        for(Vertex vertex : mesh.getVertices())
            objVertices.push_back(vertex);

    // Buffer de objetos de vertices
    unsigned int VBO;
    // Objeto de arreglo de indices de vertices
    unsigned int VAO;

    // Crear el buffer de memoria para los vertices
    glGenBuffers(1, &VBO);
    // Crear el buffer de memoria para los indices
    glGenVertexArrays(1, &VAO);

    // Asignar el buffer de memoria para los indices
    glBindVertexArray(VAO);
    // Asignar el buffer de memoria para los vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Asignar los datos del buffer de memoria para los vertices
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Asignar los datos del buffer de memoria para los indices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // Habilitar el buffer de memoria para los vertices
    glEnableVertexAttribArray(0);

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
    ventana = glfwCreateWindow(800, 600, "GLFW | Lector .obj", NULL, NULL);
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