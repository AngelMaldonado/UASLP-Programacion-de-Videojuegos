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
#include "../include/objloader.h"
// Mesh
#include "../include/mesh.h"

using namespace glm;

// Referencia global a la ventana
GLFWwindow* ventana;

// Matriz model-view-projection
mat4 mvp;
// Matriz de normales
mat4 normal;

// Funcion para inicializar la ventana con GLFW, glew y OpenGL
int InicializaVentana();
// Funcion para inicializar las matrices de transformacion
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

    // Lectura del objeto
    // Toda la inicializacion del VBO y VAO se hace en el constructor de Mesh
    Mesh mesh("../obj/test/monkey1.obj");

    GLuint programaID = LoadShaders("C:\\Users\\amald\\OneDrive - Universidad Autonoma de San Luis Potosi - UASLP\\Programacion\\Programacion de Videojuegos\\[3]LectorOBJ\\shaders\\vs1.glsl",
                                    "C:\\Users\\amald\\OneDrive - Universidad Autonoma de San Luis Potosi - UASLP\\Programacion\\Programacion de Videojuegos\\[3]LectorOBJ\\shaders\\fs1.glsl");

    
    // Ciclo principal
    do
    {
        // Limpia la ventana de color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Seleccionar el programa de shaders
        glUseProgram(programaID);

        // Variables uniformes
        // Matrices de transformacion
        genMatrices();
        // // Guarda las matrices en el buffer-shader
        int mvpLoc = glGetUniformLocation(programaID, "MVP");
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, value_ptr(mvp));
        // Normal
        int normalLoc = glGetUniformLocation(programaID, "Normal");
        glUniformMatrix4fv(normalLoc, 1, GL_FALSE, value_ptr(normal));
        // Color del modelo
        int colorLoc = glGetUniformLocation(programaID, "Color");
        glUniform3f(colorLoc, 0.0f, 0.0f, 1.0f);
        // Direccion de la luz
        // int lightDirLoc = glGetUniformLocation(programaID, "LightDir");
        // glUniform3f(lightDirLoc, 0.0f, 0.0f, 1.0f);

        // Dibuja el objeto
        // Las operaciones Bind y dibujado del objeto se hacen en esta funcion
        mesh.Draw();

        // Actualizar la ventana (buffer)
        glfwSwapBuffers(ventana);
        // Esperar una actualizacion
        glfwPollEvents();

    }while(glfwGetKey(ventana, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(ventana) == 0);

    // Liberar los recursos
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

void genMatrices() {
    // Model-view projection
    mat4 model = mat4(1); // model
    model = translate(model, vec3(0.0f, 0.0f, -5.0f));
    mat4 view = mat4(1); // view
    vec3 eye = vec3(0.0f, 1.0f, 5.0f);
    vec3 center = vec3(0.0f, 0.0f, 0.0f);
    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    view = lookAt(eye, center, up);
    mat4 projection = perspective(radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f); // projection
    
    mvp = projection * view * model;

    // Normal
    normal = model;

    // 
}