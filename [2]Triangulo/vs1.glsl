// En un vertex shader lo principal que se debe hacer es asignar la posicion de cada vertice
#version 330 core

layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 Color;

out vec3 colorVertice;

void main()
{
    //gl_position en una variable out predefinida de tipo vec4
    gl_Position = vec4(Pos.x, Pos.y, Pos.z, 1.0); // Posicion del vertice
    colorVertice = vec3(Color.x, Color.y, Color.z); // Color del vertice
}