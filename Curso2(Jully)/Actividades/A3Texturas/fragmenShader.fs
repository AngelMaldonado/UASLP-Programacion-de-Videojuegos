#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 ourTexture;
in vec2 ourTexture2;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    //FragColor = texture(texture0, ourTexture); //* vec4(ourColor, 1.0f);
    FragColor = mix(texture(texture0, ourTexture), texture(texture1, ourTexture), 0.3f);
}
