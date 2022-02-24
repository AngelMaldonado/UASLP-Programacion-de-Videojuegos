#version 330 core

//debe de existir una variable de salida que es el color final del fragmento
out vec4 FragColor;
in vec3 colorVertice;

// Declaracion de variable uniforme
uniform vec3 colorUniform;
uniform float factorAmbiental;

void main()
{
    // FragColor = vec4(colorVertice, 1.0f);
    // (colorUniform / factorAmbiental) -> divide cada color entre el factor ambiental
    FragColor = vec4(colorUniform * factorAmbiental, 1.0f);
} 