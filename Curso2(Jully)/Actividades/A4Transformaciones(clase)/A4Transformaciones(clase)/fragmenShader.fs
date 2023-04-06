#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 ourTexture1;
in vec2 ourTexture2;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragColor = mix(texture(texture1, ourTexture1), texture(texture2, ourTexture2), 0.3);
}