/**
 * @brief A new C++ implementation file
 * 
 * @author @AngelMaldonado
 * @file PruebaTextura2D.cpp
 * @link www.somelink.com
 * @version 1.0 Sat Apr 23 2022
 */

#include <GL/glew.h>

#include "PruebaTextura2D.h"

#include "../Renderizador.h"
#include "../vendor/imgui/imgui.h"

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

namespace prueba
{
    PruebaTextura2D::PruebaTextura2D()
        : m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
        m_Vista(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
        m_TraslacionA(200, 200, 0), m_TraslacionB(400, 200, 0)
    {
        float posiciones[] {
            -50.0f, -50.0f, 0.0f, 0.0f,
             50.0f, -50.0f, 1.0f, 0.0f,
             50.0f,  50.0f, 1.0f, 1.0f,
            -50.0f,  50.0f, 0.0f, 1.0f
        };

        unsigned int indices[] {
            0, 1, 2,
            2, 3, 0 
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_VAO = std::make_unique<VertexArray>();

        m_VB = std::make_unique<VertexBuffer>(posiciones, 4 * 4 * sizeof(float));       
        DistribucionVertexBuffer distribucionVbo;
        distribucionVbo.Push<float>(2);
        distribucionVbo.Push<float>(2);
        m_VAO->AgregaBuffer(*m_VB, distribucionVbo);
        
        m_IB = std::make_unique<IndexBuffer>(indices, 6);

        m_Shader = std::make_unique<Shader>("../res/shaders/Basico.shader");
        m_Shader->Bind();
        m_Shader->EstableceUniforme4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
        
        m_Textura = std::make_unique<Textura>("../res/textures/windows.png");
        m_Shader->EstableceUniforme1i("u_Textura", 0);
    }
    
    PruebaTextura2D::~PruebaTextura2D() 
    {
        
    }
    
    void PruebaTextura2D::OnUpdate(float diferenciaDeTiempo) 
    {
        
    }
    
    void PruebaTextura2D::OnRender() 
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));       
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderizador renderizador;

        m_Textura->Bind();

        {
            glm::mat4 modelo = glm::translate(glm::mat4(1.0f), m_TraslacionA);
            glm::mat4 mvp = m_Proj * m_Vista * modelo;
            m_Shader->Bind();
            m_Shader->EstableceUniformeMat4f("u_MVP", mvp);

            renderizador.Dibuja(*m_VAO, *m_IB, *m_Shader);
        }

        {
            glm::mat4 modelo = glm::translate(glm::mat4(1.0f), m_TraslacionB);
            glm::mat4 mvp = m_Proj * m_Vista * modelo;
            m_Shader->Bind();
            m_Shader->EstableceUniformeMat4f("u_MVP", mvp);

            renderizador.Dibuja(*m_VAO, *m_IB, *m_Shader);
        }
    }
    
    void PruebaTextura2D::OnImGuiRender() 
    {
        ImGui::SliderFloat3("Traslacion A", &m_TraslacionA.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("Traslacion B", &m_TraslacionB.x, 0.0f, 960.0f);
    
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}
