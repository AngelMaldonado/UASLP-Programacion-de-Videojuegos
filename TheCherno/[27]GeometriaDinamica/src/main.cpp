/**
 * @brief Hasta el momento el VB en el renderizado batch se ha quedado "estatico", por lo que
 * modificar las coordenadas de los objetos o cualquier otro atributo del VB resultaria
 * complicado, una manera de resolver este problema es declarar el VB como dinamico y no
 * pasarle ningun tipo de informacion al inicio (antes del ciclo principal), sino hasta que
 * se necesite (antes de llamar la funcion de dibujado), esto quiere decir que la informacion
 * del CPU se copia hacia la GPU.
 * 
 * @author @AngelMaldonado
 * @file main.cpp, VertexBuffer.h, VertexBuffer.cpp
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

/**
 * @brief Estructura de un vertice con los atributos que se
 * requieren para renderizar un objeto en OpenGL:
 * - Posicion: 3 flotantes (xyz)
 * - Color: 4 flotantes (RGBA)
 * - Coordenadas de textura: 2 flotantes (xy)
 * - ID de textura: 1 entero
 */
struct Vertice
{
    /**
     * @brief Posicion del vertice en x, y, z
     */
    glm::vec3 Posicion;
    /**
     * @brief Color del vertice en formato RGBA
     */
    glm::vec4 Color;
    /**
     * @brief Coordenadas de la textura x, y
     */
    glm::vec2 CoordsTextura;
    /**
     * @brief ID de la textura a la que corresponde dentro del VB
     */
    unsigned int IDTextura;
};

static std::array<Vertice, 4> CreaCuadrado(float x, float y, float IDTextura)
{
    float tamano = 100.0f;

    Vertice v0;
    v0.Posicion =           glm::vec3(x, y, 0.0f);
    v0.Color =              glm::vec4(0.18f, 0.6f, 0.96f, 1.0f);
    v0.CoordsTextura =      glm::vec2(0.0f, 0.0f);
    v0.IDTextura = IDTextura;
    
    Vertice v1;
    v1.Posicion =           glm::vec3(x + tamano, y, 0.0f);
    v1.Color =              glm::vec4(0.18f, 0.6f, 0.96f, 1.0f);
    v1.CoordsTextura =      glm::vec2(1.0f, 0.0f);
    v1.IDTextura = IDTextura;
    
    Vertice v2;
    v2.Posicion =           glm::vec3(x + tamano, y + tamano, 0.0f);
    v2.Color =              glm::vec4(0.18f, 0.6f, 0.96f, 1.0f);
    v2.CoordsTextura =      glm::vec2(1.0f, 1.0f);
    v2.IDTextura = IDTextura;
    
    Vertice v3;
    v3.Posicion =           glm::vec3(x, y + tamano, 0.0f);
    v3.Color =              glm::vec4(0.18f, 0.6f, 0.96f, 1.0f);
    v3.CoordsTextura =      glm::vec2(0.0f, 1.0f);
    v3.IDTextura = IDTextura;

    return { v0, v1, v2, v3 };
}

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

        /* Creacion del Vertex Buffer dinamico con capacidad para 100 vertices (Vertice) */
        VertexBuffer vboDinamico(sizeof(Vertice) * 1000);
        
        /* Definicion de la distribucion de los datos dentro del struct Vertice */
        DistribucionVertexBuffer distribucionVbo;
        distribucionVbo.Push<float>(3);         // Posicion
        distribucionVbo.Push<float>(4);         // Color
        distribucionVbo.Push<float>(2);         // Coordenadas de textura
        distribucionVbo.Push<unsigned int>(1);  // ID de textura
        va.AgregaBuffer(vboDinamico, distribucionVbo);
        
        IndexBuffer ibo(indices, 6 * 2);

        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 vista = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        Shader shader("../res/shaders/Basico.shader");
        shader.Bind();

        Textura textura1("../res/textures/windows7.png");
        Textura textura2("../res/textures/windows11.png");
        textura1.Bind(0);
        textura2.Bind(1);
        std::vector<int> ranurasTexturas{ 0, 1 };
        shader.EstableceUniforme1iv("u_Texturas", ranurasTexturas);

        Renderizador renderizador;

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(ventana, true);
        ImGui::StyleColorsDark();

        /* Vectores de traslacion */
        glm::vec3 traslacionA(0, 50, 0);
        glm::vec3 traslacionB(0, 100, 0);

        while(!glfwWindowShouldClose(ventana))
        {
            /* Creacion de los cuadrados */
            auto cuadrado1 = CreaCuadrado(traslacionA.x, traslacionA.y, 0.0f);
            auto cuadrado2 = CreaCuadrado(traslacionB.x, traslacionB.y, 1.0f);
            /* Creacion del buffer de los cuadrados */
            Vertice vertices[8];
            memcpy(vertices, cuadrado1.data(), cuadrado1.size() * sizeof(Vertice));
            memcpy(vertices + cuadrado1.size(), cuadrado2.data(), cuadrado2.size() * sizeof(Vertice));

            /* Bind del VB dinamico con la informacion actualizada */
            vboDinamico.BindDinamico(vertices, sizeof(vertices));

            renderizador.LimpiaBuffer();
            
            ImGui_ImplGlfwGL3_NewFrame();
            
            shader.Bind();
            glm::mat4 mvp = proj * vista; // * modelo;
            shader.EstableceUniformeMat4f("u_MVP", mvp);
            
            renderizador.Dibuja(va, ibo, shader);

            /* Controles para modificar las posiciones de los cuadrados */
            {
                ImGui::SliderFloat3("Traslacion A", &traslacionA.x, 0.0f, 960.0f);
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
