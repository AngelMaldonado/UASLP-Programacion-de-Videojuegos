/**
 * @brief El difuminado funciona como poner un cristal azul encima de
 * uno rojo, el resultado seria una combinacion de esos 2 colores; morado.
 * El difuminado en OpenGL determina como combinar el color de salida (fragment)
 * con lo que ya esta en el buffer objetivo (parte que ya tiene pixeles asignados).
 * 
 * Para hacer difuminado en OpenGL:
 * 1. Habilitar el difuminado: glEnable(GL_BLEND);
 * 2. Definir el modo de difuminado: glBlendFunc(src, dest);
 *    como se calculara el color de salida de source y dest, la idea es que src
 *    por defecto se toma como 1 y dest como 0.
 * 3. Difuminar el resultado de la funcion anterior con: glBlendEquation(modo);
 *    - modo: como se combinan el src y dest
 * 
 * Si se dejan los valores por defecto de src y dest, la funcion glBlendFunc haria:
 * 1(src) + 0(dest) = 1 (src) <- deja el color de source
 * 
 * En este programa se utiliza las opciones:
 * - src = GL_SRC_ALPHA
 * - dest = GL_ONE_MINUS_SRC_ALPHA
 * Las operaciones que se realizarian seria "usar el color de destino", ya que src es
 * transparente (ALPHA):
 * src alpha = 0, dest = 1 - 0
 * - R = (rsrc * 0) + (rdest * (1 - 0)) = rdest
 * - G = (gsrc * 0) + (gdest * (1 - 0)) = gdest
 * - B = (bsrc * 0) + (bdest * (1 - 0)) = bdest
 * - A = (asrc * 0) + (adest * (1 - 0)) = adest
 * 
 * Ejemplo:
 * El pixel src con el color (1.0, 1.0, 1.0, 0.5)
 * El pixel dest con el color (1.0, 0.0, 1.0, 1.0)
 * Con nuestro modo de difuminado, las operaciones serian:
 * - R = (1.0 * 0.5) + (1.0 * (1.0 - 0.5)) = 1.0
 * - G = (1.0 * 0.5) + (0.0 * (1.0 - 0.5)) = 0.5 <- en este canal se nota el difuminado
 * - B = (1.0 * 0.5) + (1.0 * (1.0 - 0.5)) = 1.0
 * - A = (0.5 * 0.5) + (1.0 * (1.0 - 0.5)) = 0.75 <- en este canal se nota el difuminado
 * 
 * @author @AngelMaldonado
 * @file main.cpp
 * @link https://github.com/nothings/stb/blob/master/stb_image.h
 * @link glfw: https://www.glfw.org/ (3.3)
 * @link glew: https://glew.sourceforge.io/ (7.0)
 * @version 1.0 Fri Apr 22 2022
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "Renderizador.h"
#include "DistribucionVertexBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Textura.h"

int main() {
    GLFWwindow* ventana;
    
    if(!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    ventana = glfwCreateWindow(640, 480, "Ventana", NULL, NULL);
    if(!ventana)
    {
        GLCall(glfwTerminate());
        return -1;
    }

    glfwMakeContextCurrent(ventana);

    glfwSwapInterval(1);

    if(glewInit() != GLEW_OK)
        return -1;

    std::cout << "Version de OpenGL: " << glGetString(GL_VERSION) << std::endl;
    {
        float posiciones[] {
            -0.5f, -0.5f, 0.0f, 0.0f,
             0.5f, -0.5f, 1.0f, 0.0f,
             0.5f,  0.5f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 1.0f 
        };

        unsigned int indices[] {
            0, 1, 2,
            2, 3, 0 
        };

        /* Habilitacion del difuminado */
        GLCall(glEnable(GL_BLEND));
        /* Definicion del modo de difuminado */
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        VertexBuffer vbo(posiciones, 4 * 4 * sizeof(float));
        
        DistribucionVertexBuffer distribucionVbo;
        distribucionVbo.Push<float>(2);
        distribucionVbo.Push<float>(2);
        va.AgregaBuffer(vbo, distribucionVbo);
        
        IndexBuffer ibo(indices, 6);

        Shader shader("../res/shaders/Basico.shader");
        shader.Bind();

        Textura textura("../res/textures/windows.png");
        textura.Bind();
        shader.EstableceUniforme1i("u_Textura", 0);

        va.Unbind();
        vbo.Unbind();
        ibo.Unbind();
        shader.Unbind();

        Renderizador renderizador;

        float r = 0.0f;
        float incr = 0.05f;
        while(!glfwWindowShouldClose(ventana))
        {
            renderizador.LimpiaBuffer();

            shader.Bind();
            shader.EstableceUniforme4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            renderizador.Dibuja(va, ibo, shader);

            if(r > 1.0f)
                incr = -0.05f;
            else if(r < 0.0f)
                incr = 0.05f;
            r += incr;

            GLCall(glfwSwapBuffers(ventana));

            GLCall(glfwPollEvents());
        }
    }
    glfwDestroyWindow(ventana);

    return 0;
}
