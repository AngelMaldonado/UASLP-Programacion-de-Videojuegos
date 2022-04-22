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

void main()
{
    color = vec4(0.2, 0.3, 0.8, 1.0);
}
