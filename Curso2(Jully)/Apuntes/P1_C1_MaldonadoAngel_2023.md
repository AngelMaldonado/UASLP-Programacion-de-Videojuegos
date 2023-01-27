<h1><center>Programación de Videojuegos</center></h1>
<center style="display: flex; justify-content: space-between;">
<img src="ingenieria.png" height=100>
  <ul style="list-style-type: none;">
    <li><i>Universidad Autónoma de San Luis Potosí</i></li>
    <li><i>Facultad de Ingeniería</i></li>
    <li><i>Parcial 1</i></li>
    <li><i>Angel de Jesús Maldonado Juárez</i></li>
    <li><b>Clase 1 - 26 de enero del 2023</b></li>
  </ul>
  <img src="uaslp.png" height=100>
</center>
<hr>

## ¿Qué es?
Es la creación y manejo de gráficos por computadora, en donde se centra en la lógica y la integración de todas las áreas (diseño, arte, música, etc.) implicadas en el desarrollo de un videojuego.

## ¿Por qué se tiene que configurar *OpenGL*?
Es similar a una biblioteca/librería para trabajar con el procesamiento de gráficos, y cada hardware y/o entorno es distinto.

*Se utilizará la versión de OpenGL 3*

## Pasos para mostrar algo con *OpenGL*

1. Crear la ventana.
2. Definir coordenadas de la proyección (perspectiva y ortográfica). *No tan necesario*
3. Definir los vértices de los triángulos de la cara del objeto a representar.
    $T1=V1,V2,V3$

    $T2=V1,V4,V3$
4. Definir las normales.

Existen 2 tipos de archivos para poder modelar y mostrar objetos en escena:

- **VAO** (*Vertex Array Object*): solo puede haber uno y en el están definidas las posiciones y características del objeto.
- **VBO** (*Vertex Buffer Object*): en el están las transformaciones, texturas y otras modificaciones al objeto en el VAO.

## ¿Qué es una escena?
Es un espacio en donde se pueden implicar todos los elementos relacionados con la visualización de objetos en una pantalla, puede tener solamente un menú dentro de un *canvas*.
Para poner un elemento en escena se necesitan: *posición*, *rotación*, *escalado* (TRANSFORMACIÓN).
Al objeto se le puede agregar un sprite (2D) o textura (3D), partículas, sombras, físicas, gravedad.
Un objeto en 3D tiene vértices, aristas, y caras.