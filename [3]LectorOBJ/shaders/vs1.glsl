#version 120

// Atributos definidos en IndexedModel
// Son guardados en el array en el constructor de Mesh
attribute vec3 position; // Posicion del vertice
attribute vec2 texCoord; // Atributo de coordenadas de textura
attribute vec3 normal; // Normal del vertice

// Variables para pasar al fragment shader
varying vec2 texCoord0;
varying vec3 normal0;

out vec3 FragColor;

uniform mat4 Model;
uniform mat4 MVP;
uniform mat4 Normal;
uniform vec3 lightDirection;

void main()
{
	gl_Position = MVP * vec4(position, 1.0);
	// normalTransformada = mat3(transpose(inverse(Model))) * normal;
	// FragPos = vec3(Model * vec4(position, 1.0));

	normal0 = (Normal * vec4(normal, 0.0)).xyz;
	texCoord0 = texCoord;
}