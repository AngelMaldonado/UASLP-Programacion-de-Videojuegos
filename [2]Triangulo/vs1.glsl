// En un vertex shader lo principal que se debe hacer es asignar la posicion de cada vertice
// Se ejecuta un shader por cada vertice
#version 330 core

// Variable de entrada (posicion)
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 Color;

// Esta variable puede leerse en el fragment shader
out vec4 colorVertice;

void main()
{
    //gl_position en una variable out predefinida de tipo vec4 (x, y, z, w)
    // esta variable define la posicion del vertice en el espacio de coordenadas
    gl_Position = vec4(Pos.x, Pos.y, Pos.z, 1.0);
    // Inicializar el color del vertice con el color que se le asigno en el arreglo
    colorVertice = vec4(Color.x, Color.y, Color.z, 1.0);
}