// /**
//  * @brief Renderizado batch
//  * 
//  * @author @AngelMaldonado
//  * @file main.cpp
//  * @link ImGui: https://github.com/ocornut/imgui/tree/v1.60
//  * @link glm: https://github.com/g-truc/glm
//  * @link stb_image: https://github.com/nothings/stb/blob/master/stb_image.h
//  * @link glfw: https://www.glfw.org/ (3.3)
//  * @link glew: https://glew.sourceforge.io/ (7.0)
//  * @version 1.0 Sat Apr 23 2022
//  */

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
         * @brief Para el renderizado batch los objetos se tienen que
         * definir en el mismo VB, por lo que las posiciones se deben duplicar
         * , la posicion de cada objeto ya tiene que estar definida, aun no se podria
         * aplicar la transformacion a los objetos directamente.
         */
        float posiciones[] {
            // Cuadrado 1
             100.0f, 100.0f, 0.0f, 0.0f,
             200.0f, 100.0f, 1.0f, 0.0f,
             200.0f, 200.0f, 1.0f, 1.0f,
             100.0f, 200.0f, 0.0f, 1.0f,
             
            // Cuadrado 2
             400.0f, 400.0f, 0.0f, 0.0f,
             500.0f, 400.0f, 1.0f, 0.0f,
             500.0f, 500.0f, 1.0f, 1.0f,
             400.0f, 500.0f, 0.0f, 1.0f
        };

        /**
         * @brief Tambien los indices deben duplicarse en renderizado batch
         * para poder dibujar 2 objetos
         */
        unsigned int indices[] {
            // Cuadrado 1
            0, 1, 2,
            2, 3, 0,

            // Cuadrado 2
            4, 5, 6,
            6, 7, 4 
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        /* Multiplicar la informacion del vbo x2 porque hay 2 objetos */
        VertexBuffer vbo(posiciones, 4 * 4 * 2 * sizeof(float));
        
        DistribucionVertexBuffer distribucionVbo;
        distribucionVbo.Push<float>(2);
        distribucionVbo.Push<float>(2);
        va.AgregaBuffer(vbo, distribucionVbo);
        
        /* Multiplicar la informacion del ibo x2 */
        IndexBuffer ibo(indices, 6 * 2);

        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 vista = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

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

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(ventana, true);
        ImGui::StyleColorsDark();

        /* De momento las traslaciones aun no se soportan */
        // glm::vec3 traslacionA(200, 200, 0);
        // glm::vec3 traslacionB(400, 200, 0);

        /* La matriz modelo para cada objeto es la misma por eso se crea una matriz de 1s */
        glm::mat4 modelo = glm::mat4(1.0f);
        /* El mvp de momento tambien es compartido por ambos cuadrados */
        glm::mat4 mvp = proj * vista * modelo;

        while(!glfwWindowShouldClose(ventana))
        {
            renderizador.LimpiaBuffer();
            
            // ImGui_ImplGlfwGL3_NewFrame();

            shader.Bind();
            shader.EstableceUniformeMat4f("u_MVP", mvp);

            renderizador.Dibuja(va, ibo, shader);

            /* Las posiciones de los cuadrados de momento es fija */
            // {
            //     ImGui::SliderFloat3("Traslacion A", &traslacionA.x, 0.0f, 960.0f);
            //     ImGui::SliderFloat3("Traslacion B", &traslacionB.x, 0.0f, 960.0f);
                
            //     ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            // }

            // ImGui::Render();
            // ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            GLCall(glfwSwapBuffers(ventana));

            GLCall(glfwPollEvents());
        }
    }
    // ImGui_ImplGlfwGL3_Shutdown();
    // ImGui::DestroyContext();

    glfwDestroyWindow(ventana);

    return 0;
}
