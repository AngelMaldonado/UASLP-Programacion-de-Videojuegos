#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 ourTexture1;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
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
uniform float time;

void main()
{
    vec3 ambiental = texture(material.diffuse, ourTexture1).rgb * light.ambient;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.pose - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuseMap = vec3(texture(material.diffuse, ourTexture1));
    vec3 difuse = diff * light.diffuse * diffuseMap;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininght);
    vec3 specularMap = vec3(texture(material.specular, ourTexture1));
    vec3 specular = spec * light.specular * specularMap;

    vec3 emissionMap = vec3(texture(material.emission, ourTexture1));
    vec3 emission = emissionMap * (sin(time) * 0.5f + 0.5f) * 2.0f;

    vec3 emissionMask = step(vec3(1.0f), vec3(1.0f) - specularMap);
    emission = emission * emissionMask;

    vec3 result = ambiental + difuse + specular + emission;
    FragColor = vec4(result, 1.0f);
}