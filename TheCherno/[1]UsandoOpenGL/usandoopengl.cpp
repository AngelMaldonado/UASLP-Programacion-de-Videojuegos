/**
 * Para hacer una aplicacion multiplataforma no se deben de extraer las librerias de los drivers
 * de la tarjeta de video del equipo, sino que se debe codificar una solucion para adaptar el codigo
 * a cada plataforma y utilizar funciones de OpenGL moderno.
 * Para ello existe la libreria glew (OpenGL Extension Wrangler).
 * 
 * @author @AngelMaldonado
 * @file usandoopengl.cpp
 * @link Fuente de conocimiento: https://youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2
 * @link glew: https://glew.sourceforge.io/ (7.0)
 * @version 1.0 18/abril/2022
 */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


int main(void)
{
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

    /* Bucle principal (hasta cerrar ventana) */
    while(!glfwWindowShouldClose(ventana))
    {
        /* Proceso de renderizado */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Prepara el buffer para dibujar una figura */
        glBegin(GL_TRIANGLES);
            glVertex2f(-0.5f, -0.5f);
            glVertex2f(0.0f, 0.5f);
            glVertex2f(0.5f, -0.5f);
        glEnd();

        /* Cambia los buffers delantero y trasero */
        glfwSwapBuffers(ventana);

        /* Proceso de eventos */
        glfwPollEvents();
    }

    /* Cerrar ventana */
    glfwDestroyWindow(ventana);
    return 0;
}
