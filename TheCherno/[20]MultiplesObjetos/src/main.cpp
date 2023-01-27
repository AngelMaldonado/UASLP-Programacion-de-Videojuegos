/**
 * @brief Una manera de poder renderizar mas de un objeto en OpenGL es mediante la
 * MVP, teniendo varias de estas matrices por cada objeto y un solo VertexBuffer, ya que el
 * VertexShader es el que se encarga de multiplicar estas matrices para obtener la ubicacion
 * final de los vertices. Ademas, esta solucion da la sensacion de tener mas objetos para
 * manipularlos, y el VertexBuffer quedaria fijo. Y ya que el VertexBuffer quedaria fijo,
 * este se debe "estandarizar", haciendo que todos los objetos se dibujen en el centro del
 * sistema de coordenadas de la ventana, para que la matriz MVP de cada objeto determine
 * a partir del centro en donde se va a dibujar.
 * 
 * En este programa el VertexBuffer se define como una caja de 50 x 50 (pixeles) y lo que
 * se dibuja en si es la caja con la textura 2 veces, sinembargo lo que hace que se dibuje 2
 * veces son los vectores de traslacion A y B, y estos vectores representarian 2 objetos
 * "independientes".
 * 
 * Esta solucion no es del todo optima para programas que requieran renderizar varios objetos,
 * ya que se invoca a la funcion Draw() varias veces y eso implica mucho gasto de CPU y GPU.
 * Y hay una manera de invocar una sola vez la funcion Draw, llamada Batching, que consiste
 * en introducir todos los datos de todos los objetos en un solo VertexBuffer y se modificarian
 * las posiciones del VertexBuffer.
 * 
 * @author @AngelMaldonado
 * @file main.cpp
 * @link ImGui: https://github.com/ocornut/imgui/tree/v1.60
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

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw_gl3.h"

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
        /**
         * @brief Este arreglo de posiciones pasara a convertirse el VertexBuffer principal del
         * programa, por lo tanto se debe modificar para que al multiplicar la matriz mvp se ubique
         * correctamente, por ejemplo, en el centro del sistema de coordenadas.
         */
        float posiciones[] {            //  ___________
            -50.0f, -50.0f, 0.0f, 0.0f, // |           | <-- la textura se ajustara a este cuadrado
             50.0f, -50.0f, 1.0f, 0.0f, // | 50x50(px) |
             50.0f,  50.0f, 1.0f, 1.0f, // |           |
            -50.0f,  50.0f, 0.0f, 1.0f  // |___________|
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
        /* Centrar la camara */
        glm::mat4 vista = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        Shader shader("../res/shaders/Basico.shader");
        shader.Bind();
        shader.EstableceUniforme4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        Textura textura("../res/textures/windows.png");
        textura.Bind();
        shader.EstableceUniforme1i("u_Textura", 0);

        va.Unbind();
        vbo.Unbind();
        ibo.Unbind();
        shader.Unbind();

        Renderizador renderizador;

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(ventana, true);
        ImGui::StyleColorsDark();

        glm::vec3 traslacionA(200, 200, 0);
        glm::vec3 traslacionB(400, 200, 0);

        float r = 0.0f;
        float incr = 0.05f;
        while(!glfwWindowShouldClose(ventana))
        {
            renderizador.LimpiaBuffer();
            
            ImGui_ImplGlfwGL3_NewFrame();

            /* Dibuja el primer objeto con la matriz de traslacion A */
            {
                glm::mat4 modelo = glm::translate(glm::mat4(1.0f), traslacionA);
                glm::mat4 mvp = proj * vista * modelo;
                shader.Bind(); // <- Se tiene que activar para cada objeto
                shader.EstableceUniformeMat4f("u_MVP", mvp);

                renderizador.Dibuja(va, ibo, shader);
            }

            /* Dibuja el segundo objeto con la matriz de traslacion B */
            {
                glm::mat4 modelo = glm::translate(glm::mat4(1.0f), traslacionB);
                glm::mat4 mvp = proj * vista * modelo;
                shader.Bind(); // <- Se tiene que activar para cada objeto
                shader.EstableceUniformeMat4f("u_MVP", mvp);

                renderizador.Dibuja(va, ibo, shader);
            }

            if(r > 1.0f)
                incr = -0.05f;
            else if(r < 0.0f)
                incr = 0.05f;
            r += incr;

            {
                /* Controles para el objeto A */
                ImGui::SliderFloat3("Traslacion A", &traslacionA.x, 0.0f, 960.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                
                /* Controles para el objeto B */
                ImGui::SliderFloat3("Traslacion B", &traslacionB.x, 0.0f, 960.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            GLCall(glfwSwapBuffers(ventana));

            GLCall(glfwPollEvents());
        }
    }
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(ventana);

    return 0;
}
