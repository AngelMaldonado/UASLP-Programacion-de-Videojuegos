/**
 * OpenGL es una API para crear aplicaciones gráficas, es un intermediario
 * entre el lenguaje C++ y OpenGL. Es multiplataforma, y se puede usar en
 * dispositivos móviles, computadoras, tablets, etc.
 * 
 * @author @AngelMaldonado
 * @file ventana.cpp
 * @link Fuente de conocimiento: https://youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2
 * @link glfw: https://www.glfw.org/ (3.3)
 * @version 1.0 18/abril/2022
 */
#include <GLFW/glfw3.h>


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