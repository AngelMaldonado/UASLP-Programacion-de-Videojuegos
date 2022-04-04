/******************************************
*  Autor : Angel Maldonado & Kevin Avila
*  Creado el : Vie Feb 25 2022
*  Archivo : main.cpp
*  Descripcion : Lectura de archivos .obj
*******************************************/

/*
 * Para iluminacion: 
 * - Flat: normales de cara
 * - Gouraud: normales de vertices
 * - Phong: normales del fragmento (todos los pixeles que conforman la cara)
*/
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

// Apertura de archivos de imagen
#include "../include/stb_image.h"

using namespace glm;

// Referencia global a la ventana
GLFWwindow* ventana;

// Matriz modelo
mat4 model;
// Matriz vista
mat4 view;
// Matriz proyeccion
mat4 projection;
// Variable de rotacion
float rotateFactor = 0.0f;
// Variable de escalado
float scaleFactor = 2.0f;

// Funcion para inicializar la ventana con GLFW, glew y OpenGL
int InicializaVentana();
// Funcion para inicializar las matrices de transformacion y agregarlas como uniformes
void genMatrices(GLuint program);

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

    // Inicializacion de la textura
    int width, height, numComponents;
    GLuint textureID;
    // Carga de la textura
    unsigned char* texture = stbi_load("../obj/test/bricks.jpg", &width, &height, &numComponents, 4);
    
    // Generar un identificador de textura
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);

    stbi_image_free(texture);

    GLuint phongShaders = LoadShaders("C:\\Users\\amald\\OneDrive - Universidad Autonoma de San Luis Potosi - UASLP\\Programacion\\Programacion de Videojuegos\\[3]LectorOBJ\\shaders\\phongvs.glsl",
                                    "C:\\Users\\amald\\OneDrive - Universidad Autonoma de San Luis Potosi - UASLP\\Programacion\\Programacion de Videojuegos\\[3]LectorOBJ\\shaders\\phongfs.glsl");
    
    GLuint gouraudShaders = LoadShaders("C:\\Users\\amald\\OneDrive - Universidad Autonoma de San Luis Potosi - UASLP\\Programacion\\Programacion de Videojuegos\\[3]LectorOBJ\\shaders\\gouraudvs.glsl",
                                    "C:\\Users\\amald\\OneDrive - Universidad Autonoma de San Luis Potosi - UASLP\\Programacion\\Programacion de Videojuegos\\[3]LectorOBJ\\shaders\\gouraudfs.glsl");
    
    GLuint flatShaders = LoadShaders("C:\\Users\\amald\\OneDrive - Universidad Autonoma de San Luis Potosi - UASLP\\Programacion\\Programacion de Videojuegos\\[3]LectorOBJ\\shaders\\flatvs.glsl",
                                    "C:\\Users\\amald\\OneDrive - Universidad Autonoma de San Luis Potosi - UASLP\\Programacion\\Programacion de Videojuegos\\[3]LectorOBJ\\shaders\\flatfs.glsl");
    
    // Ciclo principal
    do
    {
        // Limpia la ventana de color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Variables uniformes
        // Lee las teclas
        if(glfwGetKey(ventana, GLFW_KEY_G))
        {
            // printf("Gouraud shading\n");
			glUseProgram(gouraudShaders);
            genMatrices(gouraudShaders);
        }
		else if (glfwGetKey(ventana, GLFW_KEY_P))
        {
            // printf("Phong shading\n");
			glUseProgram(phongShaders);
            genMatrices(phongShaders);
        }
		else if (glfwGetKey(ventana, GLFW_KEY_F))
        {
            // printf("Flat shading\n");
			glUseProgram(flatShaders);
            genMatrices(flatShaders);
        }
        else
        {
            // printf("Flat shading\n");
            glUseProgram(flatShaders);
            genMatrices(flatShaders);
        }

        // Dibuja el objeto
        // Las operaciones Bind y dibujado del objeto se hacen en esta funcion
        mesh.Draw();

        // Actualizar la ventana (buffer)
        glfwSwapBuffers(ventana);
        // Esperar una actualizacion
        glfwPollEvents();

    }while(glfwGetKey(ventana, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(ventana) == 0);

    // Liberar los recursos
    glDeleteProgram(gouraudShaders);
    glDeleteProgram(phongShaders);
    glDeleteProgram(flatShaders);
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

void genMatrices(GLuint program) {
    // Model
    model = mat4(1);
    mat4 scaleM = scale(model, vec3(scaleFactor)); // scale
    mat4 translateM = translate(model, vec3(0.0f, 0.0f, -5.0f)); // translate
    mat4 rotX = rotate(mat4(1), rotateFactor += 0.0001, vec3(1.0, 0.0, 0.0)); // rotation in x
    mat4 rotY = rotate(mat4(1), rotateFactor += 0.0001, vec3(0.0, 1.0, 0.0)); // rotation in y
    mat4 rotZ = rotate(mat4(1), 0.0f, vec3(0.0, 0.0, 1.0)); // rotation in z
    mat4 rotXYZ = rotX * rotY * rotZ; // rotation in x, y, z
    model = translateM * rotXYZ * scaleM; // model
    // View
    view = mat4(1); // view
    vec3 eye = vec3(0.0f, 1.0f, 5.0f); // eye
    vec3 center = vec3(0.0f, 0.0f, 0.0f); // center
    vec3 up = vec3(0.0f, 1.0f, 0.0f); // up
    view = lookAt(eye, center, up); // view
    // Projection
    projection = perspective(radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f); // projection

    // Matriz de modelo
    int modelLoc = glGetUniformLocation(program, "Model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
    // Matriz de vista
    int viewLoc = glGetUniformLocation(program, "View");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
    // Matriz de proyeccion
    int projLoc = glGetUniformLocation(program, "Projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));

    // Color del modelo
    int colorLoc = glGetUniformLocation(program, "ObjectColor");
    glUniform3f(colorLoc, 1.0f, 0.0f, 1.0f);
    // Posicion de la camara
    int camPosLoc = glGetUniformLocation(program, "ViewPos");
    glUniform3f(camPosLoc, 0.0f, 0.0f, 1.0f);
    // Posicion de la luz
    int lightPosLoc = glGetUniformLocation(program, "LightPosition");
    glUniform3f(lightPosLoc, 0.0f, -2.0f, 2.0f);
    // Color de la luz
    int lightColLoc = glGetUniformLocation(program, "LightColor");
    glUniform3f(lightColLoc, 1.0f, 1.0f, 1.0f);
}