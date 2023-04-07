#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 ourTexture1;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininght;
};

struct Light{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 pose;
};

uniform Material material;
uniform Light light;
uniform sampler2D texture1;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vec3 ambiental = material.ambient * light.ambient;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.pose - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 difuse = (diff * material.diffuse) * light.diffuse;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininght);
    vec3 specular = spec * light.specular * material.specular;

    vec3 result = ambiental + difuse + specular;
    //FragColor = vec4(result, 1.0f);
    FragColor = texture(texture1, ourTexture1);
}