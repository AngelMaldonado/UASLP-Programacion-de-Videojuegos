#shader vertex
#version 330 core

layout (location = 0) in vec4 posicion;
// El atributo 1 sera el color
layout (location = 1) in vec4 color;
// layout (location = 1) in vec2 coordTextura;

// Variable de salida para el fragment shader}
out vec4 v_Color;
// out vec2 v_coordTextura;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * posicion;
    v_Color = color;
    // v_coordTextura = coordTextura;
}

#shader fragment
#version 330 core

layout (location = 0) out vec4 color;

// Variable de entrada para el color (desde vertex shader)
in vec4 v_Color;
// in vec2 v_coordTextura;

// uniform vec4 u_Color;
// uniform sampler2D u_Textura;

void main()
{
    // vec4 colorTextura = texture(u_Textura, v_coordTextura);
    // Definir el color final de salida al color del vertex shader
    color = v_Color;
}
