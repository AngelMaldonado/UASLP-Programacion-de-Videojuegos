// Fragment shader:
// ================
#version 330 core
out vec4 FragColor;

in vec3 LightingColor; 

uniform vec3 ObjectColor;

void main()
{
   FragColor = vec4(LightingColor * ObjectColor, 1.0);
}