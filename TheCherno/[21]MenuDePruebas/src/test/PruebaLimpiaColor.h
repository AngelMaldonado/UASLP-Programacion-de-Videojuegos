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
    class PruebaLimpiaColor : public Prueba
    {
    public:
        PruebaLimpiaColor();
        ~PruebaLimpiaColor();

        void OnUpdate(float diferenciaDeTiempo) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        float m_LimpiaColor[4];
    };
}
