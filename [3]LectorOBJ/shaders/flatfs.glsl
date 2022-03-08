#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 texCoords;
layout (location = 2) in vec3 normal;

// Normal
in vec3 Normal;
// Posicion del fragmento
in vec3 FragPos;
// Posicion de la luz
in vec3 LightPos;
// Final fragment color;
out vec4 FragColor;

// Color
uniform vec3 ObjectColor;
// Tipo de iluminacion
uniform int TypeIlum;
// Light color
uniform vec3 LightColor;
// Light
uniform vec3 LightPosition;
// Camera position
uniform vec3 ViewPos;

void main()
{
    // Se multiplica la matriz de textura por la matriz de color
	//gl_FragColor = texture2D(sampler, texCoord0) * vec4(Color, 1.0);
    FragColor = vec4((1 - max(0, dot(LightPosition, Normal))) * ObjectColor, 1.0);
}