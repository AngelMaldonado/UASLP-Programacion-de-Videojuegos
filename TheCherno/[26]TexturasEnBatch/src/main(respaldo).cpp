/**
 * @brief Para agregar mas de 1 textura en renderizado batch el primer paso es agregar
 * las posiciones de las texturas en el VB (como ya estaba definido), pero como no se
 * puede llamar la funcion de dibujado mas de una vez para establecer 2 uniformes en el
 * ciclo principal, lo que se debe hacer es definir esas 2 uniformes antes del ciclo para
 * meter al lote (batch) la informacion de las texturas, pero para que cada objeto pueda
 * identificar que textura va a utilizar se le puede agregar un indice a los atributes del
 * VB para que lo puedan utilizar los shaders y mapear tanto las coordenadas como los datos
 * del shader que segun corresponda en base a este indice.
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
        /* Agregar al VB las coordenadas de texturas */
        float posiciones[] {
            // Cuadrado 1
             100.0f, 100.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 0.0f, 0.0f, //(texturas-indice)
             200.0f, 100.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 0.0f, 0.0f,
             200.0f, 200.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 1.0f, 0.0f,
             100.0f, 200.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 1.0f, 0.0f,

            // Cuadrado 2
             400.0f, 400.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 0.0f, 1.0f,
             500.0f, 400.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 0.0f, 1.0f,
             500.0f, 500.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 1.0f, 1.0f,
             400.0f, 500.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 1.0f, 1.0f
        };

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
        /* Multiplicar flotantes x9 ya que se agregaron las texturas y sus indices */
        VertexBuffer vbo(posiciones, 4 * 9 * 2 * sizeof(float));
        
        DistribucionVertexBuffer distribucionVbo;
        distribucionVbo.Push<float>(2);
        distribucionVbo.Push<float>(4);
        /* Agregar el atributo de texturas */
        distribucionVbo.Push<float>(2);
        /* Agregar el atributo de indice de texturas */
        distribucionVbo.Push<float>(1);
        va.AgregaBuffer(vbo, distribucionVbo);
        
        IndexBuffer ibo(indices, 6 * 2);

        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 vista = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        Shader shader("../res/shaders/Basico.shader");
        shader.Bind();

        Textura textura1("../res/textures/windows7.png");
        Textura textura2("../res/textures/windows11.png");
        /* Establece la ranura para cada textura */
        textura1.Bind(0);
        textura2.Bind(1);
        /* Carga las texturas a la variable uniforme */
        std::vector<int> ranurasTexturas{ 0, 1 };
        shader.EstableceUniforme1iv("u_Texturas", ranurasTexturas);

        va.Unbind();
        vbo.Unbind();
        ibo.Unbind();
        shader.Unbind();

        Renderizador renderizador;

        // ImGui::CreateContext();
        // ImGui_ImplGlfwGL3_Init(ventana, true);
        // ImGui::StyleColorsDark();

        /* De momento las traslaciones aun no se soportan */
        // glm::vec3 traslacionA(200, 200, 0);
        // glm::vec3 traslacionB(400, 200, 0);

        glm::mat4 modelo = glm::mat4(1.0f);
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
