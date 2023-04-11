<h1><center>Programación de Videojuegos</center></h1>
<center style="display: flex; justify-content: space-between;">
<img src="img/ingenieria.png" height=100>
  <ul style="list-style-type: none;">
    <li><i>Universidad Autónoma de San Luis Potosí</i></li>
    <li><i>Facultad de Ingeniería</i></li>
    <li><i>Parcial 2</i></li>
    <li><i>Angel de Jesús Maldonado Juárez</i></li>
    <li><i>Actividad 2</i></li>
    <li><b>30 de marzo del 2023</b></li>
  </ul>
  <img src="img/uaslp.png" height=100>
</center>
<hr>

## Primer avance del proyecto
El proyecto consta de la creación de un videojuego con vista
de primera persona, en donde el jugador deberá superar un mapa
de obstáculos. Este mapa deberá basarse en el inflable
mostrado en el video [*Ultimate Survival - 395ft / 120m
Inflatable Obstacle Course*](https://www.youtube.com/watch?v=gxt3T2VlWhQ):

<center>
<img src="img/mapa-video.png" height=200>
</center>

## Modelado del mapa
Para el modelado del mapa y los obstáculos se utiliza el
programa [*Blender*](https://www.blender.org/), el avance del
mapa sin texturas ni materiales es de la siguiente forma:

<center>
<img src="img/mapa-sin-texturas.png" height=300>
</center>

Para la base del mapa se utiliza una imagen que simula
la textura y apariencia de la lona del inflable:

<center>
<img src="img/inflable.jpg" height=150>
</center>

Para la placa de meta también se utiliza una imagen con el
patrón de cuadros blanco y negro:

<center>
<img src="img/race.jpg" height=155>
</center>

Los picos y obstáculos del mapa no utilizan ninguna textura,
sin embargo, se crea un material utilizando un nodo **BSDF**
(*Bidirectional Scattering Distribution Function*), cuya
configuración queda de la siguiente forma:

<center>
<img src="img/bsdf.png" height=300>
</center>

Cada obstáculo será de un color distinto, por lo que
simplemente se optó por duplicar el primer material creado
la misma cantidad de colores que habrá en el mapa:

<center>
<img src="img/materiales.png" height=200>
</center>

Finalmente, al aplicar las texturas y materiales a cada
elemento en el mapa, este se visualiza de la siguiente
manera:

<center>
<img src="img/mapa-con-texturas.png" height=300>
</center>

Para la exportación del modelo se optó por el formato *.obj*,
por lo que se generará el archivo del modelo *.obj* junto con
el archivo que contiene los materiales *.mtl* la configuración
de exportado queda de la siguiente manera:

<center>
<img src="img/exportado.png" height=300>
</center>

## Lectura del modelo y los materiales en OpenGL

Como segundo avance del proyecto se carga el modelo del mapa
del juego utilizando *OpenGL*, la plantilla del proyecto en
*Visual Studio*, y el código visto en clases.

Para la carga del modelo simplemente se reemplaza la cadena
de la ruta de uno de los modelos que se cargan en el archivo
`main.cpp`:

<center>
<img src="img/lectura-modelo-opengl.png" height=300>
</center>

A manera de prueba, simplemente se modifica el valor de la
variable `UseTexture` en el archivo `vertexShader.vs` entre 0
y 1 para visualizar el mapa sin texturas y con color o con
texturas y sin color (respectivamente):

<center>
<img src="img/fragmentshader.png" height=300>
</center>

## Mapa sin texturas y con color (en OpenGL)
<center>
<img src="img/mapa-color-opengl.png" height=300>
</center>

*El mapa queda corto para el área de visualización
(renderizado), por lo que el mapa debe escalarse a un
tamaño más chico, o debe agrandarse el área de renderizado.*

## Mapa con texturas y sin color (en OpenGL)
<center>
<img src="img/mapa-texturas-opengl.png" height=300>
</center>
