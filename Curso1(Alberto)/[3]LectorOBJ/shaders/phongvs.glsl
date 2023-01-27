// Vertex shader:
// ================
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 texCoords;
layout (location = 2) in vec3 normal;

out vec3 FragPos;
out vec3 Normal;
out vec3 LightPos;

uniform vec3 LightPosition; // we now define the uniform in the vertex shader and pass the 'view space' lightpos to the fragment shader. lightPos is currently in world space.

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
    gl_Position = Projection * View * Model * vec4(position, 1.0);
    FragPos = vec3(View * Model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(View * Model))) * normal;
    LightPos = vec3(View * vec4(LightPosition, 1.0)); // Transform world-space light position to view-space light position
}