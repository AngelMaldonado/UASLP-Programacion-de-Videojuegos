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
// Object3D
#include "../include/Object3D.h"

// Apertura de archivos de imagen
#include "../include/stb_image.h"

using namespace glm;

// Referencia global a la ventana
GLFWwindow* ventana;

// Matrices model, view y projection
mat4 model;
mat4 view;
mat4 projection;

// Matriz de normales
mat4 normal;

// Variable de rotacion
float ang = 0.0f;

// Variables de rotacion de la camara
GLfloat RotX = 0.0f , RotY = 0.0f;

// Funcion para inicializar la ventana con GLFW, glew y OpenGL
int InicializaVentana();
// Funcion para inicializar las matrices de transformacion
void genMatrices();
// Funcion para procesar las entradas del teclado
void ProcessInput();
// Funcion para crear las matrices de vista
mat4 FPViewMatrix(vec3 pos, float rotX, float rotY);

// Main
int main(void)
{
    // Si se pudo inicializar la ventana
    if(InicializaVentana() == 0)
    {
        printf("Error al inicializar la ventana\n");
        return -1;
    }

    // Lectura del archivo obj y almacenamiento en un objeto
    ObjLoader loader;

    // Objetos individuales
    Object3D obj1, obj2;
    // Vector de objetos
    vector<Object3D> objects;
    
    // Carga el objeto 1
    obj1 = loader.LoadModel("C:\\Users\\amald\\OneDrive - Universidad Autonoma de San Luis Potosi - UASLP\\Programacion\\Programacion de Videojuegos\\[2]Ejercicios\\obj\\cubo.obj");
    // Modifica la matriz de modelo
    // obj1.ModelMatrix = rotate(obj1.ModelMatrix, radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
    // obj1.ModelMatrix = translate(obj1.ModelMatrix, vec3(0.0f, 0.0f, 0.0f));
    
    // Carga el objeto 2
    obj2 = loader.LoadModel("C:\\Users\\amald\\OneDrive - Universidad Autonoma de San Luis Potosi - UASLP\\Programacion\\Programacion de Videojuegos\\[2]Ejercicios\\obj\\plano.obj");
    // Modifica la matriz de modelo
    // obj2.ModelMatrix = rotate(obj2.ModelMatrix, radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
    // obj2.ModelMatrix = translate(obj2.ModelMatrix, vec3(0.0f, 0.0f, 0.0f));
    
    // Agrega los objetos al vector
    objects.push_back(obj1);
    objects.push_back(obj2);

    // Inicializacion de la textura
    int width, height, numComponents;
    GLuint textureID;
    // Carga de la textura
    unsigned char* texture = stbi_load("../obj/test/fire_texture.jpg", &width, &height, &numComponents, 4);

    stbi_image_free(texture);

    GLuint programaID = LoadShaders("C:\\Users\\amald\\OneDrive - Universidad Autonoma de San Luis Potosi - UASLP\\Programacion\\Programacion de Videojuegos\\[3]LectorOBJ\\shaders\\vs_ilum.glsl",
                                    "C:\\Users\\amald\\OneDrive - Universidad Autonoma de San Luis Potosi - UASLP\\Programacion\\Programacion de Videojuegos\\[3]LectorOBJ\\shaders\\fs_ilum.glsl");

    
    // Ciclo principal
    do
    {
        // Limpia la ventana de color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Procesar las entradas del teclado (1 vez por frame)
        ProcessInput();

        // Aplica el shader y modifica las matrices para cada modelo
        for(int i = 0; i < (int)objects.size(); i++)
        {
            // Obten el modelo actual
            Object3D obj = objects[i];

            // Utiliza el shader
            glUseProgram(programaID);

            // Variable uniforme de color
            int idUniform = glGetUniformLocation(programaID, "colorUniform");
            glUniform3f(idUniform, 1.0f, 0.0f, 0.0f); // Crea el color (rojo)
            // Variable uniforme de factor ambiental
            int idFactorAmb = glGetUniformLocation(programaID, "factorAmbiental");
            glUniform1f(idFactorAmb, 0.5f); // Crea el factor ambiental (0.5)

            // Genera las matrices (mvp)
            genMatrices();

            // Guarda las matrices en las variables uniformes model, view y projection
            int modelLoc = glGetUniformLocation(programaID, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(obj.ModelMatrix)); // Carga la matriz del modelo
            int viewLoc = glGetUniformLocation(programaID, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
            int projLoc = glGetUniformLocation(programaID, "projection");
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));

            // Hacer el bind de los vertices del objeto actual
            glBindVertexArray(obj.VertexArrayID);

            // Dibuja el objeto actual
            glDrawArrays(GL_TRIANGLES, 0, obj.NumberOfVertices);
        }

        

    }while(glfwGetKey(ventana, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(ventana) == 0);

    // Libera los buffers de los objetos
    for(int i = 0; i < (int)objects.size(); i++)
    {
        // Objeto actual
        Object3D obj = objects[i];

        // Libera el buffer de vertices
        glDeleteBuffers(1, &obj.VertexBufferID);
        // Libera el buffer de indices
        glDeleteBuffers(1, &obj.VertexArrayID);
    }

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
    // Modelo
    model = mat4(1);
    // translate
    model = translate(model, vec3(0.0f, 0.0f, 0.0f));
    // rotate
    model = rotate(model, radians(ang), vec3(1.0f, 1.0f, 0.0f));
    ang += 0.5f;
    if(ang > 360.0f) ang = 0.0f;

    // Vista
    // view = mat4(1);
    vec3 eye = vec3(0.0f, 0.0f, 5.0f);
    vec3 center = vec3(0.0f, 0.0f, 0.0f);
    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    view = lookAt(eye, center, up);

    // Proyeccion
    projection = perspective(radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
}

void ProcessInput()
{

}

mat4 FPViewMatrix(vec3 pos, float rotX, float rotY)
{
    mat4 view;

    // Convertir a radianes
    rotX = radians(rotX);
    rotY = radians(rotY);

    // Matriz de rotacion
    view = rotate(mat4(1), rotX, vec3(1.0, 0.0, 0.0));
    view = rotate(view, rotY, vec3(0.0, 1.0, 0.0));

    // Matriz de traslacion
    view = translate(view, pos);

    return view;
}