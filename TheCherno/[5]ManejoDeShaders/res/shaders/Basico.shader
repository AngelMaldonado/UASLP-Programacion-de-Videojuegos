#shader vertex // Tipo de shader
#version 330 core

// Acceso a la distribucion de los atributos (location = 0) <- indice del atributo //
// in <- variable de entrada //
// vec4 ya que la funcion gl_Position requiere un vec4 //
layout (location = 0) in vec4 posicion;

void main()
{
    gl_Position = posicion;
}

#shader fragment // Tipo de shader
#version 330 core

layout (location = 0) out vec4 color;

void main()
{
    color = vec4(0.2, 0.3, 0.8, 1.0);
}
