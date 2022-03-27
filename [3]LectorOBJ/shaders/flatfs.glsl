#version 330 core

// Texture coordinates
out vec2 TexCoords;
// Normal
in vec3 Normal;
// Posicion del fragmento
in vec3 FragPos;
// Posicion de la luz
in vec3 LightPos;
// Final fragment color;
out vec4 FragColor;

// Texturas
uniform sampler2D sampler;

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
    FragColor = texture(sampler, TexCoords) * vec4((1 - max(0, dot(LightPosition, Normal))) * ObjectColor, 1.0);
}