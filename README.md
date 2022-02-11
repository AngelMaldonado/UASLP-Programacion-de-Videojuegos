# Programación de videojuegos

Esta materia esta pensada para reforzar los conocimientos de las operaciones con matrices que tiene la librería *OpenGL* para la representación de gráficos 3D. Utilizando como objeto de estudio la creación de un videojuego.

## Librerías

Las librerías que serán utilizadas para los ejercicios y proyecto de esta materia son las siguientes:
* [GLFW](https://www.glfw.org/): API de *OpenGL* que tiene funciones de manejo de eventos, detección de entradas/salidas, creacion de ventanas, etc. En pocas palabras, es la biblioteca principal para dibujar gráficos en pantalla.
* [GLEW](http://glew.sourceforge.net/index.html): cuya finalidad es aumentar la compatibilidad de los ejercicios, ya que esta determina las extensiones que son soportadas en la plataforma que se ejecutan los ejercicios o proyecto.
* [GLM](https://glm.g-truc.net/0.9.9/index.html): esta biblioteca cuenta con funciones que ayudan al cálculo de las operaciones con matrices y otras operaciones que son necesarias en la graficación por computadora.

## Configuración (setup)

Para facilitar la instalación de las bibliotecas, se utiliza [MSYS2](https://www.msys2.org/), esta herramienta permite instalar bibliotecas y paquetes que son de utilidad en el desarrollo de software de una manera muy sencilla utilizando comandos.

### Instalación
1. Descargar [MSYS2](https://www.msys2.org/) y seguir los pasos indicados por el *wizard*.
*(Al elegir el directorio de instalación es recomendable una ruta sin espacios)*
2. Ejecutar *MSYS2* desde el inicio de Windows o desde el ejecutable en su directorio de instalación para instalar los paquetes necesarios.
3. Instalar [gcc](https://packages.msys2.org/package/mingw-w64-x86_64-gcc): compilador para *C/C++*, con el comando:
` pacman -S mingw-w64-x86_64-gcc`.
4. Instalar el [depurador](https://packages.msys2.org/package/mingw-w64-x86_64-gdb): con el comando: 
`pacman -S mingw-w64-x86_64-gdb`
5. Instalar [glfw](https://packages.msys2.org/package/mingw-w64-x86_64-glfw?repo=mingw64):
`pacman -S mingw-w64-x86_64-glfw`
6. Instalar [glew](https://packages.msys2.org/package/mingw-w64-x86_64-glew?repo=mingw64):
`pacman -S mingw-w64-x86_64-glew`
7. Instalar [glm](https://packages.msys2.org/package/mingw-w64-x86_64-glm):
`pacman -S mingw-w64-x86_64-glm`

Todos los paquetes y librerías se instalaron en el directorio `mingw64`, que es el directorio del compilador principal que se utiliza en los ejercicios y proyecto.
Finalmente abrir la configuración de las variables del sistema y agregar en `PATH` el directorio `C:\<ruta>\msys64\mingw64\bin` para que Windows y el editor pueda interpretar el compilador y las bibliotecas.

### Configuración de VS Code

Debido a que hay algunos ejercicios muy cortos en donde solamente se requiere tocar algun tema de la graficación, se quiere que el proceso de compilación y edición del código no tenga que verse retrasado por un *IDE* tan pesado como *Visual Studio* o *CLion*. *VS Code* es un editor de código muy ligero, multiplataforma y con excelentes extensiones.
Después de haber instalado *VS Code* de su [página oficial](https://code.visualstudio.com/), se debe instalar la extensión [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools).
Para comprobar la instalación se puede utilizar el código de ejemplo `[0]VS_Code/main.cpp`. Al abrir este directorio con el editor y seleccionar el archivo main.cpp, se puede presionar `F5` para iniciar la compilación con `g++.exe`, al seguir aparecerá un error y es porque no se han enlazado las bibliotecas de *OpenGL*. Para ello, se debe abrir el archivo que se generó al presionar `F5`, `.vscode/tasks.json` en este se debe agregar a los argumentos (`args`) después de la extensión del ejecutable (*.exe*) `"-lglfw3"` y `"-lopengl32"` en líneas separadas y antecedidas por una coma. Esto habrá enlazado las bibliotecas, entonces se debe eliminar el archivo `launch.json` (si se generó) y volver a ejecutar con `F5`. Finalmente, se ejecutará el programa y significa que se instaló el compilador y las bibliotecas correctamente.

### Ruta del compilador (posible error)

Si *VS Code* no detecta la instancia del compilador de *MSYS2* se puede agregar manualmente la ruta del compilador `mingw64`, ejecutando `Ctrl+,` y buscando "*compiler*" aparecerá la opción "*Default: Compiler Path*" para la extensión *C_Cpp* entonces en el campo se debe agregar la ruta del compilador `mingw64`.

## Ejercicios

Los ejercicios serán editados y ejecutados utilizando la herramienta [VisualStudio Code](https://code.visualstudio.com/Download), en cada directorio de los ejercicios contendrán su correspondiente archivo **task.json** para que pueda ser ejecutado utilizando la librería previamente mencionada y pueda ser ejecutado con el depurador por defecto de *VisualStudio Code*. 

## Proyecto

El proyecto se desarrollará utilizando el IDE de JetBrains [CLion](https://www.jetbrains.com/clion/), de la misma forma que los ejercicios. El archivo CMake ya tendrá los enlazadores para que funcione utilizando las bibliotecas de *freeglut*.