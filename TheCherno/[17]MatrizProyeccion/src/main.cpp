/**
 * @brief La matriz de proyeccion funciona para "mapear" de alguna forma
 * las coordenadas que se han definido para aparecer correctamente en la
 * ventana; estas coordenadas se deben "proyectar" en la imagen 2D de la
 * ventana en el monitor, ya sea en un mundo 3D o 2D.
 * Este mapeo se realiza en un espacio "normalizado", el cual esta definido
 * como un sistema de coordenadas de -1 a 1 (en todos los ejes). Si no se
 * tuviera la matriz de proyeccion para normalizar las coordenadas de los
 * vertices, cualquier vertice fuera de este sistema de coordenadas (-1 a 1)
 * simplemente no se visualizaria, entonces, la matriz de proyeccion es una
 * ayuda para nosotros poder definir nuestro propio sistema de coordenadas
 * comodamente y que esta matriz convierta este sistema al espacio normalizado.
 * 
 * Los tipos de proyeccion se aplican a esta matriz, y hay 2 tipos:
 * - Ortografica: los objetos no son afectados por la distancia de alejamiento,
 *   siempre se muestran con su tamano real.
 * - Perpectiva: Los objetos si son afectados por la distancia de alejamiento,
 *   si estan lejos se muestran pequenos, si estan cerca se muestran mas grande.
 * 
 * @author @AngelMaldonado
 * @file main.cpp
 * @link glm: https://github.com/g-truc/glm
 * @link stb_image: https://github.com/nothings/stb/blob/master/stb_image.h
 * @link glfw: https://www.glfw.org/ (3.3)
 * @link glew: https://glew.sourceforge.io/ (7.0)
 * @version 1.0 Sat Apr 23 2022
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
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

int main() {
    GLFWwindow* ventana;
    
    if(!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Se aumenta la resolucion de la ventana de 640 x 480 (4 : 3) a 960 x 540 (2 : 3)*/
    ventana = glfwCreateWindow(960, 540, "Ventana", NULL, NULL);
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
        /** 
         * Como ahora se tiene un sistema de coordenadas basado en pixeles, las posiciones
         * de los vertices deben estar acordes a este sistema 
         */
        float posiciones[] {
            100.0f, 100.0f, 0.0f, 0.0f,
            200.0f, 100.0f, 1.0f, 0.0f,
            200.0f, 200.0f, 1.0f, 1.0f,
            100.0f, 200.0f, 0.0f, 1.0f
        };

        unsigned int indices[] {
            0, 1, 2,
            2, 3, 0 
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        VertexBuffer vbo(posiciones, 4 * 4 * sizeof(float));
        
        DistribucionVertexBuffer distribucionVbo;
        distribucionVbo.Push<float>(2);
        distribucionVbo.Push<float>(2);
        va.AgregaBuffer(vbo, distribucionVbo);
        
        IndexBuffer ibo(indices, 6);

        /** 
         * Matriz de proyeccion ortografica, defe definirse de acuerdo al aspecto de la ventana 
         * , al definirla se puede establecer el sistema de coordenadas que se requiera. En este
         * ejemplo se definen los limites de la pantalla desde la parte inferior-izquierda hasta
         * la parte superior-derecha con la resolucion 960 x 540 (misma resolucion de la ventana).
         */
        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);

        // -> Debugear y verificar como se transforman sus coordenadas al sistema normalizado
        glm::vec4 vp(100.0f, 100.0f, 0.0f, 1.0f); // la ultima coordenada siempre es 1.0f (eje w)
        glm::vec4 result = proj * vp; // <- Esta operacion se realiza en el vertex shader

        Shader shader("../res/shaders/Basico.shader");
        shader.Bind();
        shader.EstableceUniforme4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
        shader.EstableceUniformeMat4f("u_MVP", proj);

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
