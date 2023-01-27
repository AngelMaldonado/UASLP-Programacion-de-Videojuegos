/**
 * @brief A new C++ header file
 * 
 * @author Author
 * @file PruebaLimpiaColor.h
 * @link www.somelink.com
 * @version 1.0 Sat Apr 23 2022
 */

#pragma once

#include "Prueba.h"

#include "../VertexBuffer.h"
#include "../DistribucionVertexBuffer.h"
#include "../Textura.h"

#include <memory>

namespace prueba {
    /**
     * @brief Crea un GUI con un seleccionador de color el cual prueba
     * la funcion glClearColor(), si cambia el color correctamente quiere
     * decir que funciona correctamente.
     * 
     * Dentro del ciclo principal llamar las siguientes funciones como en el
     * ejemplo:
     * 
     * prueba.OnUpdate(0.0f);
     * OnRender();
     * 
     * - Frame ImGui
     * 
     * prueba.OnImGuiRender();
     */
    class PruebaTextura2D : public Prueba
    {
    public:
        PruebaTextura2D();
        ~PruebaTextura2D();

        void OnUpdate(float diferenciaDeTiempo) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<VertexBuffer> m_VB;
        std::unique_ptr<IndexBuffer> m_IB;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Textura> m_Textura;

        glm::mat4 m_Proj;
        glm::mat4 m_Vista;
        glm::vec3 m_TraslacionA;
        glm::vec3 m_TraslacionB;
    };
}
