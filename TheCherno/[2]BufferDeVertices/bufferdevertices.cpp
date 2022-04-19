/**
 * @brief Para que un modelo o figura pueda mostrarse en OpenGL moderno, debe establecerse:
 * - Un buffer de vertices: es un arreglo de bytes en memoria que almacena datos de los vertices.
 *   este buffer se almacena en la GPU gracias a OpenGL.
 * - Shaders para los vertices: es una secuencia de instrucciones que procesan los datos de los vertices
 *   y se ejecutan en la GPU.
 * OpenGL funciona como una maquina de estados, donde el estado actual es determinado en base a la seleccion
 * del buffer que querramos que se muestre en pantalla.
 * 
 * @author @AngelMaldonado
 * @file bufferdevertices.cpp
 * @link glfw: https://www.glfw.org/ (3.3)
 * @link glew: https://glew.sourceforge.io/ (7.0)
 * @version 1.0 Mon Apr 18 2022
 * 
 * @link Enlace para documentacion de OpenGL: https://docs.gl/
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

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


    /* Bucle principal (hasta cerrar ventana) */
    while(!glfwWindowShouldClose(ventana))
    {
        /* Proceso de renderizado */
        glClear(GL_COLOR_BUFFER_BIT);

        /**
         * @brief Una vez definido el buffer con los datos, y OpenGL ya tiene conocimiento
         * de este buffer, se debe especificar lo que significa cada dato dentro del buffer.
         * De esto se encargan los shaders, que son secuencias de instrucciones que procesan
         * los datos del buffer.
         * En este programa simplemente estamos "simulando" el dibujado del buffer con la
         * funcion glDrawArrays, pero debido a que no hay shadres, no se mostrara nada en
         * la ventana.
         */
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Cambia los buffers delantero y trasero */
        glfwSwapBuffers(ventana);

        /* Proceso de eventos */
        glfwPollEvents();
    }

    /* Cerrar ventana */
    glfwDestroyWindow(ventana);
    return 0;

    return 0;
}
