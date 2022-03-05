#version 330 core

// Ya se interpolan las normales
varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 position0;
varying vec3 faceNormal;
varying vec3 fragColor;

uniform sampler2D sampler;
uniform vec3 lightDirection;
uniform vec3 Color;
uniform int typeIlum;

// Funciones de iluminacion
vec4 flatIlum(vec3 objectColor);
vec4 phongIlum(vec3 objectColor);

void main()
{
    // Se multiplica la matriz de textura por la matriz de color
	//gl_FragColor = texture2D(sampler, texCoord0) * vec4(Color, 1.0);

    // Mostrar solamente la texura
    //gl_FragColor = texture2D(sampler, texCoord0);
	
    // Mostrar solamente el color
    //gl_FragColor = vec4(Color, 1.0);

    // Color, textura e iluminacion
    //gl_FragColor = texture2D(sampler, texCoord0) * vec4(Color, 1.0) * clamp(dot(-lightDirection, normal0), 0.0, 1.0);

    switch(typeIlum)
    {
        // Solo color
        case 1:
            gl_FragColor =  vec4(fragColor, 1.0); // * texture2D(sampler, texCoord0);
        break;
        // Iluminacion phong
        case 2:
            gl_FragColor =  phongIlum(fragColor); // * texture2D(sampler, texCoord0);
        break;
        // Iluminacion flat
        case 3:
            gl_FragColor =  flatIlum(fragColor); // * texture2D(sampler, texCoord0);
        break;
        // Default
        default:
            gl_FragColor =  vec4(fragColor, 1.0); // * texture2D(sampler, texCoord0);
        break;
    }
}

vec4 phongIlum(vec3 objectColor){

    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 viewPos = vec3(0.0f, 0.0f, 6.0f);

    //ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    //diffuse
    vec3 norm = faceNormal;
    normalize(lightDirection);
    vec3 lightDir = lightDirection - position0;
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos) - position0;
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * objectColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;

    return vec4(result, 1.0);
}

vec4 flatIlum(vec3 objectColor){

    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 viewPos = vec3(0.0f, 0.0f, 6.0f);

    //ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    //diffuse
    vec3 norm = faceNormal;
    normalize(lightDirection);
    vec3 lightDir = lightDirection - position0;
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos) - position0;
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * objectColor;

    return vec4 (((ambient + diffuse + specular) * objectColor), 1.0f);
}