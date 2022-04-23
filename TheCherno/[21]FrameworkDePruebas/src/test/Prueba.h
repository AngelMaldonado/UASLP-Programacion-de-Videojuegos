/**
 * @brief A new C++ header file
 * 
 * @author @AngelMaldonado
 * @file Prueba.h
 * @link www.somelink.com
 * @version 1.0 Sat Apr 23 2022
 */

#pragma once

namespace prueba {
    class Prueba
    {
    public:
        /**
         * @brief Constructor de Prueba
         */
        Prueba() {}

        /**
         * @brief Destructor de Prueba
         */
        virtual ~Prueba() {}

        virtual void OnUpdate(float diferenciaDeTiempo) {}
        virtual void OnRender() {}
        virtual void OnImGuiRender() {}
    };
}
