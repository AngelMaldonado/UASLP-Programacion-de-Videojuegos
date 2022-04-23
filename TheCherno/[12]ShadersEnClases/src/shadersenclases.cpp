/**
 * @brief Abstraccion del codigo de shader en clases.
 * 
 * @author @AngelMaldonado
 * @file shadersenclases.cpp
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
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
/* Incluye el nuevo archivo Shader.h */
#include "Shader.h"

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
        -0.5f, -0.5f,
            0.5f, -0.5f,
            0.5f,  0.5f,
        -0.5f,  0.5f,
        };

        unsigned int indices[] {
            0, 1, 2,
            2, 3, 0 
        };

        VertexArray va;
        VertexBuffer vbo(posiciones, 4 * 2 * sizeof(float));
        
        DistribucionVertexBuffer distribucionVbo;
        distribucionVbo.Push<float>(2);
        va.AgregaBuffer(vbo, distribucionVbo);
        
        IndexBuffer ibo(indices, 6);

        /* Crea el shader */
        Shader shader("../res/shaders/Basico.shader");
        /* Activa el shader (bind) */
        shader.Bind();
        /* Crea la variable uniforme para el color */
        shader.EstableceUniforme4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        va.Unbind();
        vbo.Unbind();
        ibo.Unbind();
        shader.Unbind();

        float r = 0.0f;
        float incr = 0.05f;
        while(!glfwWindowShouldClose(ventana))
        {

            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            /* Activa el shader */
            shader.Bind();
            /* Actualiza la variable uniforme */
            shader.EstableceUniforme4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            va.Bind();
            ibo.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

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
