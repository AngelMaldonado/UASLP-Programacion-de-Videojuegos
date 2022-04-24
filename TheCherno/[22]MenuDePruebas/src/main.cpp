/**
 * @brief Las pruebas estan en el directorio test
 * 
 * @author @AngelMaldonado
 * @file main.cpp, Prueba.h, Prueba.cpp
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

#include "test/PruebaLimpiaColor.h"

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
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        Renderizador renderizador;

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(ventana, true);
        ImGui::StyleColorsDark();

        /* Apuntador a la prueba activa actual */
        prueba::Prueba* pruebaActual = nullptr;
        prueba::MenuDePruebas* menuDePruebas = new prueba::MenuDePruebas(pruebaActual);
        pruebaActual = menuDePruebas;

        /* Agrega al menu de pruebas PruebaLimpiaColor */
        menuDePruebas->RegistraPrueba<prueba::PruebaLimpiaColor>("Limpia Color");

        while(!glfwWindowShouldClose(ventana))
        {
            GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
            renderizador.LimpiaBuffer();

            ImGui_ImplGlfwGL3_NewFrame();
            /* Si prueba actual no es nulo */
            if (pruebaActual)
            {
                pruebaActual->OnUpdate(0.0f);
                pruebaActual->OnRender();
                /* Inicia la ventana con el nombre Prueba */
                ImGui::Begin("Prueba");
                /* Si se presiono el boton de volver */
                if (pruebaActual != menuDePruebas && ImGui::Button("<-"))
                {
                    /* Regresar al menu de pruebas */
                    delete pruebaActual;
                    pruebaActual = menuDePruebas;
                }
                /* Renderizar la prueba actual o el menu */
                pruebaActual->OnImGuiRender();
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            GLCall(glfwSwapBuffers(ventana));

            GLCall(glfwPollEvents());
        }
        if(pruebaActual != menuDePruebas)
            delete menuDePruebas;
        delete pruebaActual;
    }
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(ventana);

    return 0;
}
