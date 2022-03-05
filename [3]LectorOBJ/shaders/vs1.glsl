#version 330 core

// Atributos definidos en IndexedModel
// Son guardados en el array en el constructor de Mesh
attribute vec3 position; // Posicion del vertice
attribute vec2 texCoord; // Atributo de coordenadas de textura
attribute vec3 normal; // Normal del vertice

// Variables para pasar al fragment shader
varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 position0;
varying vec3 faceNormal;
varying vec3 fragColor;

uniform mat4 Model;
uniform mat4 MVP;
uniform mat4 Normal;
uniform vec3 Color;
uniform vec3 lightDirection;
uniform int typeIlum;

// Iluminacion gouraud
void gouraudIlum(vec3 objectColor);

void main()
{
    // Iguala el color al fragColor
    fragColor = Color;
    position0 = position;

	gl_Position = MVP * vec4(position, 1.0);

	if(typeIlum == 1)
		gouraudIlum(fragColor);

    normal0 = normal;
	texCoord0 = texCoord;
	faceNormal = mat3(transpose(inverse(Model))) * normal; // Normal de la cara
}

void gouraudIlum(vec3 objectColor){

	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 viewPos = vec3(0.0f, 0.0f, 6.0f);

    //ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    //diffuse
    vec3 norm = normalize(mat3(Model) * normal);
    vec3 lightDir = normalize(lightDirection) - position;
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos) - position;
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * objectColor;

    objectColor = (ambient + diffuse + specular) * objectColor;
}