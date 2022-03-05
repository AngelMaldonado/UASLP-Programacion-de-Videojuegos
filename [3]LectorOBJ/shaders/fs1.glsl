#version 120

// Ya se interpolan las normales
varying vec2 texCoord0;
varying vec3 normal0;

uniform sampler2D sampler;
uniform vec3 lightDirection;
uniform vec3 Color;

void main()
{
    // Se multiplica la matriz de textura por la matriz de color
	gl_FragColor = texture2D(sampler, texCoord0) * vec4(Color, 1.0);

    // Mostrar solamente la texura
    //gl_FragColor = texture2D(sampler, texCoord0);
	
    // Mostrar solamente el color
    //gl_FragColor = vec4(Color, 1.0);

    // Color, textura e iluminacion
    //gl_FragColor = texture2D(sampler, texCoord0) * vec4(Color, 1.0) * clamp(dot(-lightDirection, normal0), 0.0, 1.0);
}