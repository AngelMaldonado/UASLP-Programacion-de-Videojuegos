// Los fragment shader se ejecutan por cada pixel en pantalla, y dibujan los objetos en pantalla.
// Deben estar en comunicacion con los vertex shader para saber como dibujar cada objeto.
#version 330 core

// Debe de existir una variable de salida que es el color final del fragmento
out vec4 FragColor;

// Recibe la variable del color
in vec4 colorVertice; // Tiene que tener el mismo nombre que el del vertex shader

void main()
{
    FragColor = vec4(colorVertice.x, colorVertice.y, colorVertice.z, 1.0f); // RGBA (0 - 1, 0 - 1, 0 - 1, 0 - 1)
}