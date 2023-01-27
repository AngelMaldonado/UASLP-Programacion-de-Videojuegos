#version 330

// Variable de salida para mostrar el color final
out vec4 FragColorOut;

// Variables de entrada
in vec3 FragPos;
in vec3 FragNormal;
in vec3 FragColor;
in vec2 FragTexCoord;

// Funcion para calcular la iluminacion
vec4 Light(vec3 objectColor);

// Main
void main()
{
    FragColorOut = vec4(FragColor, 1.0f);
}

vec4 Light(vec3 objectColor)
{
    // Color de la luz (blanco)
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    // Posicion de la luz
    vec3 lightPos = vec3(1.2f, 1.0f, 5.0f);
    // Posicion de la vista
    vec3 viewPos = vec3(0.0f, 0.0f, -4.0f);

    // Factor ambiental
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    // Factor difuso
    vec3 norm = normalize(FragNormal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;

    // Factor especular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Final
    vec3 result = (ambient + diffuse + specular) * objectColor;
    return vec4(result, 1.0f);
}