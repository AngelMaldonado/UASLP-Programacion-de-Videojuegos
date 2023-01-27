/**
 * @brief La matriz MVP (Model-View-Projection) es el resultado de multiplicar
 * cada matriz individual:
 * 
 * - Model: casi siempre es una matriz de 1s, esta matriz define 3 aspectos basicos
 *   de un modelo 3D o 2D: traslacion, rotacion y escalado. Con esta matriz se puede 
 *   modificar cualquiera de los aspectos para manipular el objeto.
 * 
 * - View: basicamente es la "camara" en la escena, se define como se ve la escena
 *   en base a los parametros de la camara.
 * 
 * - Projection: matriz que define la proyeccion de las posiciones en el sistema
 *   normalizado, que es un sistema de coordenadas de -1 a 1 en todos los ejes.
 *   Puede ser una proyeccion ortografica o en perspectiva
 * 
 * En OpenGL la multiplicacion se realiza de forma inversa (PVM) ya que debe
 * posicionarse en el buffer de memoria en la GPU mediante el shader.
 * 
 * Estos conceptos son "imaginarios" en OpenGL, ya que al final lo unico que se
 * hace es multiplicar todas las matrices y reducir todo a una sola matriz por
 * vertice y eso es lo que se muestra en pantalla.
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

        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        /**
         * Matriz vista (view), se define como una matriz de traslacion porque
         * es como "funcionaria" una camara en el contexto de OpenGL, ya que si
         * la camara se mueve a la derecha, los objetos en la escena deberian
         * "aparentar" moverse a la izquierda y viceversa.
         * En este ejemplo la camara se mueve a la derecha 100 pixeles, por lo que
         * la escena y todos sus objetos deben moverse -100 pixeles (a la izquierda)
         */
        glm::mat4 vista = glm::translate(glm::mat4(1), glm::vec3(-100, 0, 0));
        /**
         * Matriz modelo (model), tambien se define como una matriz de traslacion,
         * y de momento solo realizaria esa operacion en el objeto que se dibujara
         * (traslacion) en 200 pixeles a la derecha y 200 pixeles hacia arriba.
         * ->Para esta matriz los valores afectan directamente al modelo.
         */
        glm::mat4 modelo = glm::translate(glm::mat4(1), glm::vec3(200, 200, 0));

        /* Matriz MVP */
        glm::mat4 mvp = proj * vista * modelo;

        Shader shader("../res/shaders/Basico.shader");
        shader.Bind();
        shader.EstableceUniforme4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
        /* Se pasa la matriz mvp para la variable uniforme */
        shader.EstableceUniformeMat4f("u_MVP", mvp);

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
