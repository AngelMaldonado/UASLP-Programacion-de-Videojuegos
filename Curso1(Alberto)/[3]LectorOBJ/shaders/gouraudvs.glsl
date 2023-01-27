// Vertex shader:
// ================
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 texCoords;
layout (location = 2) in vec3 normal;

out vec3 LightingColor; // resulting color from lighting calculations

uniform vec3 LightPosition;
uniform vec3 ViewPosition;
uniform vec3 LightColor;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
    gl_Position = Projection * View * Model * vec4(position, 1.0);
    
    // gouraud shading
    // ------------------------
    vec3 Position = vec3(Model * vec4(position, 1.0));
    vec3 Normal = mat3(transpose(inverse(Model))) * normal;
    
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * LightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPosition - Position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * LightColor;
    
    // specular
    float specularStrength = 1.0; // this is set higher to better show the effect of Gouraud shading 
    vec3 viewDir = normalize(ViewPosition - Position);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * LightColor;      

    LightingColor = ambient + diffuse + specular;
}