#shader vertex
#version 330 core

layout (location = 0) in vec4 posicion;
layout (location = 1) in vec2 coordTextura;

out vec2 v_coordTextura;

/** @brief Variable uniforme Model-View-Projection, de momento
 *  solo guarda la matriz de proyeccion definida en main.cpp
 */
uniform mat4 u_MVP;

void main()
{
    /** Multiplicacion de la matriz MVP por la posicion para la correcta ubicacion de
     *  las coordenadas de los vertices segun la matriz de proyeccion.
     */
    gl_Position = u_MVP * posicion;
    v_coordTextura = coordTextura;
}

#shader fragment
#version 330 core

layout (location = 0) out vec4 color;

in vec2 v_coordTextura;

uniform vec4 u_Color;
uniform sampler2D u_Textura;

void main()
{
    vec4 colorTextura = texture(u_Textura, v_coordTextura);
    color = colorTextura;
}
