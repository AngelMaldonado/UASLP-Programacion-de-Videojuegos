#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 texCoords;
layout (location = 2) in vec3 normal;

out vec3 Normal;
out vec3 FragPos;
out vec3 LightPos;

// Model
uniform mat4 Model;
// View
uniform mat4 View;
// Projection
uniform mat4 Projection;
// Light
uniform vec3 LightPosition;


void main()
{
    // Calcula la posicion
    gl_Position = Projection * View * Model * vec4(position, 1.0);
    // Normal de la cara
    Normal = mat3(transpose(inverse(Model))) * normal;
    // Posicion del fragmento
    FragPos = vec3(Model * vec4(position, 1.0));
    // Posicion de la luz
    LightPos = vec3(View * vec4(LightPosition, 1.0));
}