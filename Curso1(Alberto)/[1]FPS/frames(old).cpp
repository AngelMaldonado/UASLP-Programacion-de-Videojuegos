/******************************************
*  Autor : Angel de Jesus Maldonado Juarez   
*  Creado el : Vie Feb 04 2022
*  Archivo : frames.cpp
*  Descripcion : implementacion de FPS
*******************************************/

#include <GL/glu.h>
#include <GL/glut.h>
#include <time.h>
#include <iostream>
#include "timer.h"

using namespace std;

// Factor de rotado
GLfloat rotacion = 0.0;
// Limite de FPS
int FPS = 60;
// Duracion de cada frame
float durFrame = (1.0 / (float)FPS);
// Declaracion del cronometro
Timer* cronometro = Timer::Instance();

// Cambia el factor de rotacion
void spin()
{
    // Actualiza el factor de rotacion
    rotacion = rotacion + 0.05;
    // Si se pasa de 360 se pone a 0
    if(rotacion > 360)
        rotacion = 0;

    // Redibuja la ventana
    glutPostRedisplay();
}

// Dibuja un cubo en la escena
void Cubo()
{
    // Limpia el bufer de color y el bufer de profundidad
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Selecciona la matriz de proyeccion
    glLoadIdentity();

    // Rota la escena
    glRotatef(rotacion, 1, 1, 0);

    //Multi-colored side - FRONT
    glBegin(GL_POLYGON);

    glColor3f( 1.0, 0.0, 0.0 );     glVertex3f(  0.5, -0.5, -0.5 );      // P1 is red
    glColor3f( 0.0, 1.0, 0.0 );     glVertex3f(  0.5,  0.5, -0.5 );      // P2 is green
    glColor3f( 0.0, 0.0, 1.0 );     glVertex3f( -0.5,  0.5, -0.5 );      // P3 is blue
    glColor3f( 1.0, 0.0, 1.0 );     glVertex3f( -0.5, -0.5, -0.5 );      // P4 is purple

    glEnd();

    // Purple side - RIGHT
    glBegin(GL_POLYGON);

    glColor3f( 1.0, 0.0, 0.0 );     glVertex3f(  0.5, -0.5, -0.5 );      // P1 is red
    glColor3f( 0.0, 1.0, 0.0 );     glVertex3f(  0.5,  0.5, -0.5 );      // P2 is green
    glColor3f( 1.0, 0.0, 1.0 );     glVertex3f(  0.5,  0.5,  0.5 );      // P3 is purple
    glColor3f( 1.0, 1.0, 1.0 );     glVertex3f(  0.5, -0.5,  0.5 );      // P4 is white

    glEnd();

    // White side - BACK
    glBegin(GL_POLYGON);

    glColor3f( 1.0, 1.0, 1.0 );     glVertex3f(  0.5, -0.5,  0.5 );      // P1 is white
    glColor3f( 1.0, 0.0, 1.0 );     glVertex3f(  0.5,  0.5,  0.5 );      // P2 is purple
    glColor3f( 0.0, 0.0, 1.0 );     glVertex3f( -0.5,  0.5,  0.5 );      // P3 is blue
    glColor3f( 0.0, 1.0, 0.0 );     glVertex3f( -0.5, -0.5,  0.5 );      // P4 is green

    glEnd();


    // Green side - LEFT
    glBegin(GL_POLYGON);

    glColor3f( 0.0, 1.0, 0.0 );     glVertex3f( -0.5, -0.5,  0.5 );      // P1 is green
    glColor3f( 1.0, 1.0, 1.0 );     glVertex3f( -0.5,  0.5,  0.5 );      // P2 is white
    glColor3f( 1.0, 0.0, 1.0 );     glVertex3f( -0.5,  0.5, -0.5 );      // P3 is purple
    glColor3f( 0.0, 0.0, 1.0 );     glVertex3f( -0.5, -0.5, -0.5 );      // P4 is blue

    glEnd();

    // Blue side - TOP
    glBegin(GL_POLYGON);

    glColor3f( 0.0, 0.0, 1.0 );     glVertex3f(  0.5,  0.5,  0.5 );      // P1 is blue
    glColor3f( 1.0, 0.0, 1.0 );     glVertex3f(  0.5,  0.5, -0.5 );      // P2 is purple
    glColor3f( 0.0, 1.0, 1.0 );     glVertex3f( -0.5,  0.5, -0.5 );      // P3 is white
    glColor3f( 0.0, 1.0, 0.0 );     glVertex3f( -0.5,  0.5,  0.5 );      // P4 is green

    glEnd();

    // Red side - BOTTOM
    glBegin(GL_POLYGON);

    glColor3f( 1.0, 0.0, 0.0 );     glVertex3f(  0.5, -0.5, -0.5 );      // P1 is red
    glColor3f( 1.0, 1.0, 1.0 );     glVertex3f(  0.5, -0.5,  0.5 );      // P2 is white
    glColor3f( 0.0, 1.0, 0.0 );     glVertex3f( -0.5, -0.5,  0.5 );      // P3 is green
    glColor3f( 0.0, 0.0, 1.0 );     glVertex3f( -0.5, -0.5, -0.5 );      // P4 is blue

    glEnd();
}

// Funcion de inicializacion del programa
void inicializa()
{
    glClearColor(0, 0, 0, 1);
    glColor3f(1, 0, 0);
    // Habilita el z-bufer
    glEnable(GL_DEPTH_TEST);
}

// Funcion de dibujado de la ventana
void dibuja()
{
    /************************************************/
    /*              𝙞𝙣𝙞𝙘𝙞𝙤 𝙙𝙚𝙡 𝙛𝙧𝙖𝙢𝙚
    /***********************************************/
    cronometro->Start();

    // Dibuja un cubo
    Cubo();
    
    cronometro->Tick();

    /************************************************/
    /*              𝙛𝙞𝙣 𝙙𝙚𝙡 𝙛𝙧𝙖𝙢𝙚
    /***********************************************/
    // Si el tiempo transcurrido es menor a lo que debe de durar el frame
    if (cronometro->DeltaTime() < durFrame)
        // Espera el tiempo que falta para que el frame se dibuje
        cronometro->Wait(durFrame - cronometro->DeltaTime());

    // Si el tiempo transcurrido es mayor o igual a lo que debe durar el frame
    if (cronometro->DeltaTime() >= durFrame)
    {
        // Mostrar los FPS
        cout << "FPS: " << 1 / cronometro->DeltaTime() << " (" << cronometro->DeltaTime() << " ms)" << endl;
        // Reiniciar el cronometro
        cronometro->Reset();
    }

    // Actualiza el buffer de color
    glutSwapBuffers();
}

int main(int c, char **v)
{
    // Inicializacion de OpenGL
    glutInit(&c, v);

    // Posicion de la ventana
    glutInitWindowPosition(250, 50);
    /* Bufer de color
     * Bufer de dibujado:
        GLUT_SINGLE -> 2D
        GLUT_DOUBLE -> 3D
     * Bufer de profundidad
        (maneja las coordenadas en Z)
    */
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    // Crea la ventana
    glutCreateWindow("Ejercicio 1 | FPS");

    // Funcion propia de inicializacion
    inicializa();
    // Funcion propia de dibujado
    glutDisplayFunc(dibuja);
    // Funcion propia de actualizacion
    glutIdleFunc(spin);

    // Inicia el ciclo de dibujado
    glutMainLoop();

    return 0;
}