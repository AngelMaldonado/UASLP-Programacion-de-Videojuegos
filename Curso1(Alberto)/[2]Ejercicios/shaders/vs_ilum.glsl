#version 330 core

// Atributos de los vertices
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

// Variables de salida (se crean en vs y se pasan a fs)
out vec3 FragPos;
out vec3 FragNormal;
out vec3 FragColor;
out vec2 FragTexCoord;

// Variables uniformes (mismas en vs y fs)
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 color;

// Main
void main()
{
	// Variable para transformar la normal
	vec3 normalTransformada;

	// Calcular la posicion y asignarla a gl_Position
	gl_Position = projection * view * model * vec4(pos, 1.0);

	// Calcular la normal transformada
	normalTransformada = mat3(transpose(inverse(model))) * normal;

	// Asignar a las variables de salida
	FragPos = vec3(model * vec4(pos, 1.0));
	FragNormal = normalTransformada;
	FragColor = color;
	FragTexCoord = texCoord;
}