#version 330 core

//debe de existir una variable de salida que es el color final del fragmento
out vec4 FragColor;
in vec3 colorVertice;

void main()
{
    FragColor = vec4(colorVertice, 1.0f);
} 