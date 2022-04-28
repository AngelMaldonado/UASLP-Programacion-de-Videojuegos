/******************************************
 *  Autor : Angel Maldonado & Kevin Avila
 *  Creado el : Vie Feb 25 2022
 *  Archivo : main.cpp
 *  Descripcion : Controlador primera persona
 *******************************************/

// TODO: mover correctamente la camara en base al modelo.
// TODO: implementar la funcion forward manualmente (up, right, forward, direction)
// TODO: https://learnopengl.com/Getting-started/Camera

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../include/shader.hpp"
#include "../include/objloader.h"
#include "../include/mesh.h"

using namespace glm;

/* Referencia a la ventana global */
GLFWwindow *ventana;

/* Arreglo de objetos */
vector<Mesh> objetos;

// Matriz vista
mat4 view;
// Matriz proyeccion
mat4 projection;

// Offset entre camara y personaje
float offset = 0.0f;

// Camara
// Rotacion de la camara
float RotX, RotY;
vec3 movCamara = vec3(0.0f, -0.5f, offset);

// Movimiento del personaje
float speed = 0.01f;
vec3 movPersonaje = vec3(0.0f, 0.5f, 0.0f);
mat4 escaladoPersonaje = scale(mat4(1.0f), vec3(0.059f));

// Funcion para inicializar la ventana con GLFW, glew y OpenGL
int InicializaVentana();
// Funcion para inicializar las matrices de transformacion y agregarlas como uniformes
void genMatrices();
// Funcion para procesar las entradas
void procesaEntrada();
// Funcion para generar la vista FP
mat4 FPViewMatrix();

// Main
int main(void)
{
    // Si se pudo inicializar la ventana
    if (InicializaVentana() == 0)
    {
        printf("Error al inicializar la ventana\n");
        return -1;
    }

    Mesh arma = Mesh("../obj/test/arma.obj");
    arma.ModelMatrix = scale(arma.ModelMatrix, vec3(0.12f));
    arma.ModelMatrix = translate(arma.ModelMatrix, movPersonaje);
    // std::cout << "Posicion del arma x:" << movPersonaje.x << " y:" << movPersonaje.y << " z:" << movPersonaje.z;
    // std::cout << "\tPosicion del camara x:" << movCamara.x << " y:" << movCamara.y << " z:" << movCamara.z << endl;
    Mesh suelo = Mesh("../obj/test/suelo.obj");
    suelo.ModelMatrix = scale(suelo.ModelMatrix, vec3(1.0));
    Mesh cubo = Mesh("../obj/test/cubo.obj");
    cubo.ModelMatrix = translate(cubo.ModelMatrix, vec3(0.0f, 0.5f, -10.0f));
    objetos.push_back(arma);
    objetos.push_back(suelo);
    objetos.push_back(cubo);

    GLuint gouraudShaders = LoadShaders("C:\\Users\\amald\\OneDrive - Universidad Autonoma de San Luis Potosi - UASLP\\Programacion\\Programacion de Videojuegos\\[3]LectorOBJ\\shaders\\gouraudvs.glsl",
                                        "C:\\Users\\amald\\OneDrive - Universidad Autonoma de San Luis Potosi - UASLP\\Programacion\\Programacion de Videojuegos\\[3]LectorOBJ\\shaders\\gouraudfs.glsl");

    // Ciclo principal
    do
    {
        // Limpia la ventana de color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Procesa la entrada del usuario
        procesaEntrada();

        // Variables uniformes
        // Color del modelo
        int colorLoc = glGetUniformLocation(gouraudShaders, "ObjectColor");
        glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);
        // Posicion de la camara
        int camPosLoc = glGetUniformLocation(gouraudShaders, "ViewPos");
        glUniform3f(camPosLoc, 0.0f, 0.0f, 1.0f);
        // Posicion de la luz
        int lightPosLoc = glGetUniformLocation(gouraudShaders, "LightPosition");
        glUniform3f(lightPosLoc, 0.0f, -4.0f, 10.0f);
        // Color de la luz
        int lightColLoc = glGetUniformLocation(gouraudShaders, "LightColor");
        glUniform3f(lightColLoc, 1.0f, 1.0f, 1.0f);

        genMatrices();
        // Procesa los modelos
        for (int i = 0; i < (int)objetos.size(); i++)
        {
            glUseProgram(gouraudShaders);

            // Variables uniformes
            // Matriz de modelo
            int modelLoc = glGetUniformLocation(gouraudShaders, "Model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(objetos[i].ModelMatrix));
            // Matriz de vista
            int viewLoc = glGetUniformLocation(gouraudShaders, "View");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
            // Matriz de proyeccion
            int projLoc = glGetUniformLocation(gouraudShaders, "Projection");
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));

            objetos[i].Draw();
        }

        // Actualizar la ventana (buffer)
        glfwSwapBuffers(ventana);
        // Esperar una actualizacion
        glfwPollEvents();
    } while (glfwGetKey(ventana, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(ventana) == 0);

    // Liberar los recursos de los modelos
    for (int i = 0; i < (int)objetos.size(); i++)
        objetos[i].~Mesh();

    // Liberar los recursos de los shaders
    glDeleteProgram(gouraudShaders);
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

mat4 FPViewMatrix()
{
    vec3 forward;
    vec3 right;
    mat4 view = mat4(1.0f);

    mat4 model = mat4(1.0f);
    // Convierte las rotaciones a radianes
    RotX = radians(RotX);
    RotY = radians(RotY);

    // Transforma la matriz de la vista en base a la rotacion del mouse
    view = rotate(view, RotX, vec3(1.0f, 0.0f, 0.0f));
    view = rotate(view, RotY, vec3(0.0f, 1.0f, 0.0f));

    // Eje Z del sistema de la camara; z es el objetivo
    forward = vec3(-view[2].x, 0.0f, view[2].z);
    // Eje X del sistema de la camara; z es la direccion de la camara
    right = vec3(view[0].x, 0.0f, -view[0].z);

    // std::cout.precision(3);
    // std::cout << "fwd: " << forward.x << ", " << forward.y << ", " << forward.z;
    // std::cout << "\trght: " << right.x << ", " << right.y << ", " << right.z << endl;
    // std::cout << "fwdM: " << forwardModel.x << ", " << forwardModel.y << ", " << forwardModel.z;
    // std::cout << "\trghtM: " << rightModel.x << ", " << rightModel.y << ", " << rightModel.z;

    if (glfwGetKey(ventana, GLFW_KEY_W) == GLFW_PRESS)
        movCamara += forward * speed, movPersonaje -= forward * speed;
    if (glfwGetKey(ventana, GLFW_KEY_S) == GLFW_PRESS)
        movCamara -= forward * speed, movPersonaje += forward * speed;
    if (glfwGetKey(ventana, GLFW_KEY_D) == GLFW_PRESS)
        movCamara -= right * speed, movPersonaje += right * speed;
    if (glfwGetKey(ventana, GLFW_KEY_A) == GLFW_PRESS)
        movCamara += right * speed, movPersonaje -= right * speed;

    // std::cout << "\tPosicion del arma x:" << movPersonaje.x << " y:" << movPersonaje.y << " z:" << movPersonaje.z;
    // std::cout << "\tPosicion del camara x:" << movCamara.x << " y:" << movCamara.y << " z:" << movCamara.z << endl;

    // Primera persona
    view = translate(view, movCamara);

    // Transforma la matriz del modelo T*R*S
    model = translate(model, -movCamara + vec3(0.0f, -0.3f, offset));
    model = rotate(model, -RotX, vec3(1.0f, 0.0f, 0.0f));
    model = rotate(model, -RotY, vec3(0.0f, 1.0f, 0.0f));
    model = model * escaladoPersonaje;
    objetos[0].ModelMatrix = model;

    return view;
}

void genMatrices()
{
    view = FPViewMatrix();

    // Projection
    projection = perspective(radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f); // projection
}

void procesaEntrada()
{
    // Si presiono la tecla ESC, cerrar la ventana
    if (glfwGetKey(ventana, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(ventana, 1);

    // Capturar el movimiento del cursor
    double xpos, ypos;
    glfwGetCursorPos(ventana, &xpos, &ypos);
    RotX = (float)ypos;
    RotY = (float)xpos;

    // Bloquea el rango de la camara
    if (RotX <= -40.0f)
        RotX = -40.0f;
    if (RotX >= 40.0f)
        RotX = 40.0f;
}