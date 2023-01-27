# Ejercicio 1 | Cuadros por segundo

*FPS* (Frames per second) es la cantidad de imágenes consecutivas que se muestran en pantalla por cada segundo mientras se juega a un videojuego o se reproduce algún producto multimedia en una pantalla.

## Ejercicio FPS

El siguiente pseudocódigo es una secuencia de las acciones que se realizan en cualquier función de dibujado en los videojuegos.
En base a este pseudocódigo se debe implementar utilizando *C/C++* y la librería de *OpenGL*.

    void main()
    {
        //Inicializacion
        int FPS = 60;
        int contFrames = 0;
        float inicial, final;
        float segInicial, segFinal;
        
        float durFrame = (1.0 / (float)FPS) * 1000.0; //16.66666667
        
        //Ciclo del juego
        
        segInicial = getTime();
        while(true)
        {
            ////Inicio de frame
            inicial = getTime(); //en milisegundos
            
            draw(); //limitar el numero de veces que se manda llamar draw() por segundo
            
            ////Fin de frame
            final = getTime();
            
            float tiempoTranscurrido = final - inicial; //cuantos ms duro el frame
            if (tiempoTranscurrido < durFrame)
            {
                //durFrame = 16.66667
                //tiempoTranscurrido = 12
                //diff = 4.66667
                
                float diff = durFrame - tiempoTranscurrido;
                inicial = getTime();//5
                while(true)
                {
                    final = getTime();//10
                    
                    if (final - inicial >= diff) break;
                }
            }
            
            contFrames++;
            segFinal = getTime();
            
            if (segFinal - segInicial >= 60)
            {
                printf(contFrames);
                segInicial = getTime();
                contFrames=0;
            }
            
            
            
            if(se presiono una accion de salir) break; //Puede ir donde quieran
        }
        
        //Cleanup
    }

### Cronómetro de alta precisión

Para medir el tiempo se utilizó como referencia el video [C++ Tutorials: Game Timer using Chrono - YouTube](https://www.youtube.com/watch?v=Vyt-dM8gTRQ), en el cual explica cómo crear un cronómetro utilizando objetos. Para adaptar esta implementación del cronómetro al ejercicio, se movió la función de iniciar el cronómetro en el momento en el que se crea el objeto (*constructor*) a un método propio llamado `Start()`.
La manera en la que un objeto `Timer` mide el tiempo es:
1. Iniciando el cronómetro con el método `Start()` donde guarda el tiempo en el que se inició con la función `std::chrono::system_clock::now()` y se guarda en la variable `mStartTime`.
2. Indicar un tick o fin de un intervalo de tiempo con el método `Tick()` donde calcula el tiempo transcurrido desde que se inició el cronómetro realizando la operación: `std::chrono::system_clock::now() -  mStartTime` y almacena el resultado en la variable `mDeltaTime` la cual podrá ser consultada por la instancia de `Time` mediante el método `DeltaTime()`, esta última regresa el tiempo transcurrido como flotante.
3. Para reiniciar el conteo simplemente es invocando al método `Reset()` el cual reestablece la marca de `mStartTime` al valor actual del tiempo.

### Ejemplo de objeto en rotación

Para que el ejercicio sea mejor ejemplificado, se tomó un ejemplo básico desarrollado con *GLFW* donde se crea un cubo y se rota a una determinada velocidad ([C++ OpenGL, GLFW Drawing a simple cube (newbedev.com)](https://newbedev.com/c-opengl-glfw-drawing-a-simple-cube)).

### Implementación de FPS

En la función `display()` dentro del ciclo `while` es donde se da todo el proceso de dibujado, por lo que toda la inicialización de las variables y constantes para la medición de los *FPS* debe ser antes de que inicie el ciclo:

    // Limite de FPS
    int  FPS = 60;
    // Duracion de cada frame
    float  durFrame = (1.0 / (float)FPS);
    // Declaracion del cronometro
    Timer* cronometro = Timer::Instance();
    
    while(!glfwWindowShouldClose(window))
    { ... }

Así entonces, la primera instrucción dentro del while es la que inicia el cronómetro, y cuando termine la secuencia de dibujado del cubo debe finalizar el cronómetro: 

    // INICIO DEL FRAME
    cronometro->Start();
    instrucciones de dibujado...
    // FIN DEL FRAME
    cronometro->Tick();

Finalmente, se debe evaluar si el tiempo que se tardó en dibujar el frame coincide con lo que en realidad debería de tardarse, si no coincide entonces debe haber un pequeño retardo para que el programa espere el tiempo restante:

    if (cronometro->DeltaTime() < durFrame)
	    cronometro->Wait(durFrame - cronometro->DeltaTime());

Donde la función `cronometro->Wait()` realiza lo siguiente:

    void  Timer::Wait(float  ms) {
	    Timer* delay = Timer::Instance();
	    while (delay->DeltaTime() < ms) delay->Tick();
    }

1. Crea un segundo cronómetro temporal para medir el tiempo que debe esperar, este tiempo se pasa como parámetro (`ms`).
2. Crea un ciclo `while` hasta que `delay->DeltaTime()` sea igual o sobrepase el tiempo que debe esperar.

Por último, cuando haya terminado la espera y dibujado del frame, se muestra en consola el cálculo de *FPS* y el tiempo que se tardó en dibujar el frame y resetear el cronómetro para el siguiente frame:

    // Mostrar los FPS y el tiempo transcurrido
    printf("FPS: %f (%.2fms) \n", 1.0 / cronometro->DeltaTime(), cronometro->DeltaTime() * 1000);
    // Reiniciar el cronometro
    cronometro->Reset();

*Nota: debido a que no se nos requirió implementar el caso en el que el frame haya tenido retraso en dibujarse, se optó en omitir dicho condicional en el código.*