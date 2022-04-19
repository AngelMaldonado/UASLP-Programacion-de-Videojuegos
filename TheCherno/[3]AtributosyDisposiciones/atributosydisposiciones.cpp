/**
 * @brief La disposicion de los atributos en la memoria de la GPU al prepararla
 * debe tener sentido para nosotros, para que posteriormente se le pueda hacer saber
 * a la GPU que atributos se van a usar y de que manera esta distribuida la 
 * informacion/atributos en el(los) buffer(s).
 * 
 * @author @AngelMaldonado
 * @file atributosydisposiciones.cpp
 * @link glfw: https://www.glfw.org/ (3.3)
 * @link glew: https://glew.sourceforge.io/ (7.0)
 * @version 1.0 Mon Apr 18 2022
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    GLFWwindow* ventana;
    
    /* Inicializar la ventana */
    if(!glfwInit())
        return -1;

    
    /* Crear la ventana y el contexto en OpenGL */
    ventana = glfwCreateWindow(640, 480, "Ventana", NULL, NULL);
    /* Si no se pudo crear la ventana */
    if(!ventana)
    {
        glfwTerminate();
        return -1;
    }

    /* Hacer ventana el contexto actual */
    glfwMakeContextCurrent(ventana);

    /* Inicializar glew despues de haber creado un contexto */
    if(glewInit() != GLEW_OK)
        return -1;

    /* Mostrar informacion de la version de OpenGL */
    std::cout << "Version de OpenGL: " << glGetString(GL_VERSION) << std::endl;

    /**
     * @brief Primeramente se define el buffer con los datos (bytes) que se le
     * pasaran a OpenGL.
     */
    /* Buffer con las posiciones de un triangulo */
    float posiciones[6] {
       -0.5f, -0.5f,
        0.0f,  0.5f,
        0.5f, -0.5f
    };
    /* ID del buffer, con este ID OpenGL sabra localizar el buffer en cuestion */
    unsigned int bufferID;
    /* Crear el buffer (1) */
    glGenBuffers(1, &bufferID);
    /* Selecciona el buffer recien credo como el buffer actual */
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
        /* Una vez seleccionado, se puede manipular los datos dentro del buffer */
        // STATIC_DRAW: los datos no cambian pero si se muestran
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, posiciones, GL_STATIC_DRAW);

        /**
         * @brief Una vez definido el buffer con los datos, y OpenGL ya tiene conocimiento
         * de este buffer, se debe especificar lo que significa cada dato dentro del buffer.
         * Una manera de hacer esto es especificando la distribucion de los datos mediante
         * indices, donde cada indice representara un atributo de los vertices.
         * La funcion glVertexAttribPointer le especifica a OpenGL la distribucion del buffer
         * y recibe como parametros:
         * - El indice del atributo (0, 1, 2, 3, 4, 5, etc)
         * - El numero de componentes de cada atributo (2, 3, 4, etc)
         * - El tipo de dato de cada componente (GL_FLOAT, GL_INT, etc)
         * - Si el dato es normalizado (GL_TRUE, GL_FALSE) -> datos decimales entre 0 y 1
         * - El offset (en bytes) entre cada conjunto de atributos de vertices [0, 6, 12, etc]
         * - El apuntador al buffer con los datos
         */
        /* Habilitar el atributo */
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    /* Bucle principal (hasta cerrar ventana) */
    while(!glfwWindowShouldClose(ventana))
    {
        /* Proceso de renderizado */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Cambia los buffers delantero y trasero */
        glfwSwapBuffers(ventana);

        /* Proceso de eventos */
        glfwPollEvents();
    }

    /* Cerrar ventana */
    glfwDestroyWindow(ventana);
    return 0;

    return 0;
}
