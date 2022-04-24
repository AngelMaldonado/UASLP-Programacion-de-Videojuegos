/**
 * @brief A new C++ implementation file
 * 
 * @author @AngelMaldonado
 * @file Prueba.cpp
 * @link www.somelink.com
 * @version 1.0 Sat Apr 23 2022
 */

#include "Prueba.h"
#include "../vendor/imgui/imgui.h"

namespace prueba
{
    MenuDePruebas::MenuDePruebas(Prueba*& apuntadorPruebaActual) 
        : m_PruebaActual(apuntadorPruebaActual)
    {
    }
    
    void MenuDePruebas::OnImGuiRender() 
    {
        for(auto& prueba : m_Pruebas)
        {
            if (ImGui::Button(prueba.first.c_str()))
                m_PruebaActual = prueba.second();
        }
    }
}