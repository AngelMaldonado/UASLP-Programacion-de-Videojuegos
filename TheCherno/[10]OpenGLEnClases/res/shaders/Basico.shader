#shader vertex
#version 330 core

layout (location = 0) in vec4 posicion;

void main()
{
    gl_Position = posicion;
}

#shader fragment
#version 330 core

layout (location = 0) out vec4 color;

// Variable uniforme para el color
uniform vec4 u_Color;

void main()
{
    // Asignar el color de salida a la variable uniforme
    color = u_Color;
}
