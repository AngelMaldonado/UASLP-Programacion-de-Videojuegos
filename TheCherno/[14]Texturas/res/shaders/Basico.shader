#shader vertex
#version 330 core

layout (location = 0) in vec4 posicion;
/* Atributo para las coordenadas de las texturas */
layout (location = 1) in vec2 coordTextura;

/* Variable de salida para el FragmentShader */
out vec2 v_coordTextura;

void main()
{
    gl_Position = posicion;
    /* Iguala las coordenadas de la textura del atributo a la variable de salida */
    v_coordTextura = coordTextura;
}

#shader fragment
#version 330 core

layout (location = 0) out vec4 color;

/* Variable de entrada para las coordenadas de textura desde el VertexShader */
in vec2 v_coordTextura;

uniform vec4 u_Color;
/* Uniforme para la textura (buffer) */
uniform sampler2D u_Textura;

void main()
{
    /* Indica que coordenada del buffer de la textura dibujar en el pixel (fragment) */
    vec4 colorTextura = texture(u_Textura, v_coordTextura);
    /* Actualizar el color con el de la textura */
    color = colorTextura;
}
