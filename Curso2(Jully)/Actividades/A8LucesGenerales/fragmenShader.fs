#version 330 core

#define NR_P_L 4

struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 direction;
};

struct PointLight{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    vec3 pose;
};

struct SpotLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 pose;
    vec3 direction;
    float cutOff;
    float outerCutOff;
};

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 ourTexture;

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_P_L];
uniform SpotLight spotLight;

uniform vec3 viewPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result;
    
    result = CalcDirLight(dirLight, norm, viewDir);

    for(int i = 0; i < NR_P_L; i++)
    {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);  
        
    FragColor = vec4(result, 1.0f);    
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    vec3 ambiental = texture(material.diffuse, ourTexture).rgb * light.ambient;
    vec3 diffuseMap = vec3(texture(material.diffuse, ourTexture));
    vec3 diffuse = diff * light.diffuse * diffuseMap;
    vec3 specularMap = vec3(texture(material.specular, ourTexture));
    vec3 specular = spec * light.specular * specularMap;

    return (ambiental + diffuse + specular);    
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.pose - fragPos);
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    float distance = length(light.pose - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambiental = texture(material.diffuse, ourTexture).rgb * light.ambient;
    vec3 diffuseMap = vec3(texture(material.diffuse, ourTexture));
    vec3 diffuse = diff * light.diffuse * diffuseMap;
    vec3 specularMap = vec3(texture(material.specular, ourTexture));
    vec3 specular = spec * light.specular * specularMap;
    
    ambiental *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
      
    return (ambiental + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.pose - fragPos);
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    float distance = length(light.pose - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambiental = texture(material.diffuse, ourTexture).rgb * light.ambient;
    vec3 diffuseMap = vec3(texture(material.diffuse, ourTexture));
    vec3 diffuse = diff * light.diffuse * diffuseMap;
    vec3 specularMap = vec3(texture(material.specular, ourTexture));
    vec3 specular = spec * light.specular * specularMap;

    ambiental *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
  
    return (ambiental + diffuse + specular);
}

