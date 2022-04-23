/**
 * @brief Las texturas pueden ser imagenes que pueden cargarse como un buffer a la GPU
 * y asi poder utilizarlas en el shader y aplicarlas a un objeto en particular, ya sea
 * para simplemente aplicar la textura como una "mascara" al objeto o para calcular efectos
 * de iluminacion en el objeto.
 * 
 * Para aplicar texturas a partir de un archivo de imagen (jpg, png, etc) se debe cargar
 * la informacion del archivo a la CPU, posteriormente este buffer debe ser enviado a la GPU
 * mediante OpenGL, y a partir de ahi el shader puede aplicar la textura al objeto utilizando
 * el buffer de la GPU.
 * 
 * Para cargar la imagen se utilizara el archivo stb_image.h, obtenido de:
 * @link https://github.com/nothings/stb/blob/master/stb_image.h
 * 
 * @author @AngelMaldonado
 * @file main.cpp, stb_image.cpp, Basico.shader, Textura.cpp
 * @link stb_image: https://github.com/nothings/stb/blob/master/stb_image.h
 * @link glfw: https://www.glfw.org/ (3.3)
 * @link glew: https://glew.sourceforge.io/ (7.0)
 * @version 1.0 Fri Apr 22 2022
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

int main() {
    GLFWwindow* ventana;
    
    if(!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    ventana = glfwCreateWindow(640, 480, "Ventana", NULL, NULL);
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
        /* Se debe agregar al VertexBuffer las coordenadas de textura */
        float posiciones[] {
            -0.5f, -0.5f, 0.0f, 0.0f, // texcoord (abajo-izquierda)
             0.5f, -0.5f, 1.0f, 0.0f, // texcoord (abajo-derecha)
             0.5f,  0.5f, 1.0f, 1.0f, // texcoord (arriba-derecha)
            -0.5f,  0.5f, 0.0f, 1.0f  // texcoord (arriba-izquierda)
        };

        unsigned int indices[] {
            0, 1, 2,
            2, 3, 0 
        };

        /* Funcion para blend (temporal) */
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        /* Actualizar a 4 flotantes por vertices */
        VertexBuffer vbo(posiciones, 4 * 4 * sizeof(float));
        
        DistribucionVertexBuffer distribucionVbo;
        distribucionVbo.Push<float>(2);
        /* Agregar 2 flotantes a la distribucion de datos del vbo */
        distribucionVbo.Push<float>(2);
        va.AgregaBuffer(vbo, distribucionVbo);
        
        IndexBuffer ibo(indices, 6);

        Shader shader("../res/shaders/Basico.shader");
        shader.Bind();

        /* Creacion de la textura */
        Textura textura("../res/textures/windows.png");
        /* Cargar la textura */
        textura.Bind();
        /* Enlazar la textura al shader con una variable uniforme con la ranura de la textura */
        shader.EstableceUniforme1i("u_Textura", 0); // 0 es la ranura de textura por defecto

        va.Unbind();
        vbo.Unbind();
        ibo.Unbind();
        shader.Unbind();

        Renderizador renderizador;

        float r = 0.0f;
        float incr = 0.05f;
        while(!glfwWindowShouldClose(ventana))
        {
            renderizador.LimpiaBuffer();

            shader.Bind();
            shader.EstableceUniforme4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            renderizador.Dibuja(va, ibo, shader);

            if(r > 1.0f)
                incr = -0.05f;
            else if(r < 0.0f)
                incr = 0.05f;
            r += incr;

            GLCall(glfwSwapBuffers(ventana));

            GLCall(glfwPollEvents());
        }
    }
    glfwDestroyWindow(ventana);

    return 0;
}
