/**
 * @brief A new C++ header file
 * 
 * @author @AngelMaldonado
 * @file Prueba.h
 * @link www.somelink.com
 * @version 1.0 Sat Apr 23 2022
 */

#pragma once

#include <iostream>
#include <vector>
#include <functional>

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

    class MenuDePruebas : public Prueba
    {
    public:
        /**
         * @brief Constructor de MenuDePruebas
         */
        MenuDePruebas(Prueba*& apuntadorPruebaActual);

        /** Debido a que las funciones virtuales tienen {}, se pueden
         *  omitir aquellas que no se necesiten
         */
        
        /**
         * @brief Renderiza el menu de pruebas
         */
        void OnImGuiRender() override;

        /**
         * @brief Funcion para crear una instancia de prueba en el menu de pruebas 
         * @param T Es el tipo de clase de la nueva prueba
         * @param nombrePrueba Nombre de la nueva prueba
         * @return Crea una instancia en el menu de pruebas dentro del vector m_Pruebas,
         * con el nombre de la prueba y el apuntador a la funcion que ejecuta la prueba.
         */
        template<typename T>
        void RegistraPrueba(const std::string& nombrePrueba)
        {
            std::cout << "Registrando una nueva prueba " << nombrePrueba << std::endl;

            m_Pruebas.push_back(std::make_pair(nombrePrueba, []() { return new T(); }));
        }
    private:
        /**
         * @brief Direccion del apuntador a la prueba activa actual
         */
        Prueba*& m_PruebaActual;

        /**
         * @brief Vector con las pruebas disponibles para elegir, cuando se
         * active alguna prueba se creara por el apuntador a la funcion Prueba*(),
         * cada posicion del vector tiene tanto el nombre de la prueba como su
         * apuntador a la funcion
         */
        std::vector<std::pair<std::string, std::function<Prueba*()>>> m_Pruebas;
    };
}
