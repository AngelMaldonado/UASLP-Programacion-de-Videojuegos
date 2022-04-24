/**
 * @brief A new C++ implementation file
 * 
 * @author @AngelMaldonado
 * @file PruebaLimpiaColor.cpp
 * @link www.somelink.com
 * @version 1.0 Sat Apr 23 2022
 */

#include <GL/glew.h>
#include "../Renderizador.h"
#include "../vendor/imgui/imgui.h"

#include "PruebaLimpiaColor.h"

namespace prueba
{
    PruebaLimpiaColor::PruebaLimpiaColor() 
        : m_LimpiaColor { 0.2f, 0.3f, 0.8f, 1.0f }
    {
        
    }
    
    PruebaLimpiaColor::~PruebaLimpiaColor() 
    {
        
    }
    
    void PruebaLimpiaColor::OnUpdate(float diferenciaDeTiempo) 
    {
        
    }
    
    void PruebaLimpiaColor::OnRender() 
    {
        GLCall(glClearColor(m_LimpiaColor[0], m_LimpiaColor[1], m_LimpiaColor[2], m_LimpiaColor[3]));       
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }
    
    void PruebaLimpiaColor::OnImGuiRender() 
    {
        ImGui::ColorEdit4("Limpia Color", m_LimpiaColor);        
    }
}
