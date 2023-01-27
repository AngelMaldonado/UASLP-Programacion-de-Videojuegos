#shader vertex
#version 330 core

layout (location = 0) in vec4 posicion;
layout (location = 1) in vec2 coordTextura;

out vec2 v_coordTextura;

void main()
{
    gl_Position = posicion;
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
