#shader vertex
#version 330 core

layout (location = 0) in vec4 posicion;
layout (location = 1) in vec4 color;
// Coordenadas de textura
layout (location = 2) in vec2 coordTextura;
// Indice para las coordenadas de las texturas
layout (location = 3) in float indiceTextura;

out vec4 v_Color;
// Variable de salida para el fragment shader de las texturas
out vec2 v_CoordTextura;
// Variable de salida para el fragment shader del indice de las texturas
out float v_IndiceTextura;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * posicion;
    v_Color = color;
    // Asignando la variable de salida de las texturas
    v_CoordTextura = coordTextura;
    // Asignando la variable de salida del indice de textura
    v_IndiceTextura = indiceTextura;
}

#shader fragment
#version 330 core

layout (location = 0) out vec4 color;

in vec4 v_Color;
// Variable de entrada para las texturas (desde vertex shader)
in vec2 v_CoordTextura;
// Variable de entrada para el indice de las texturas (desde vertex shader)
in float v_IndiceTextura;

// uniform vec4 u_Color;
// Arreglo con las texturas
uniform sampler2D u_Texturas[2];

void main()
{
    int indiceTextura = int(v_IndiceTextura);
    color = texture(u_Texturas[indiceTextura], v_CoordTextura);
}
