#pragma once
#include <GL/glu.h>

#define DEF_WINDOW_WIDTH 500
#define DEF_WINDOW_HEIGHT 500
#define DEF_WINDOW_SIZE 500
#define DEF_WINDOW_POSX 50
#define DEF_WINDOW_POSY 50
#define DEF_WINDOW_POSXY 50
#define DEF_WINDOW_NAME "OpenGL - Custom framework App"

/**
 * To use this interface subclasses are necessary to use the callbacks:
 * example: 
 * SomeSubclass *mySubClass = new SomeSubClass();
 * GLInterface *glInterface = new GLInterface();
 * 
 * mySubClass->someOperations(void someParameters);
 * glInterface->setView();
 * 
 * glInterface->setInstance(mySubClass);
 * 
 * glInterface->startFramework();
 */

// Enum with the different views for frameworks (2D and 3D)
enum View {
    VIEW_3D,
    VIEW_2D,
    VIEW_PERSP
};

// Brush to paint the line/curve
typedef struct {
    // Red value for Brush
    int red;
    // Green value for Brush
    int green;
    // Blue value for Brush
    int blue;
    // Pixel size for the Brush
    float pointSize; 
    // Drawing style for the Brush
    GLenum lineStyle;
} Brush;

// Structure for the instensity of the light in RGB
typedef struct {
    float R;
    float G;
    float B;
} IRGB;

class GLInterface {
    protected:
        /*
         * This is an instance pointer of GLInterface class to be able to instantiante the
         * function callbacks.
         */
        static GLInterface *instance;
    private:
        // Window's width in pixels
        int wndWith;
        // Window's height in pixels
        int wndHeight;
        // Window's X position in pixels
        int wndPosX;
        // Window's Y position in pixels
        int wndPosY;
        // Window's name
        char* wndName;
        // Background red value
        float backgroundR;
        // Background green value
        float backgroundG;
        // Background blue value
        float backgroundB;

        // Viewing matrix mode (OpenGL)
        GLenum matrixMode;
        // Ortho left value in pixels (OpenGL)
        float orthoVleft;
        // Ortho right value in pixels (OpenGL)
        float orthoVright;
        // Ortho bottom value in pixels (OpenGL)
        float orthoVbottom;
        // Ortho top value in pixels (OpenGL)
        float orthoVtop;
        // Perspective left value in pixels (OpenGL)
        float perspVleft;
        // Perspective right value in pixels (OpenGL)
        float perspVright;
        // Perspective bottom value in pixels (OpenGL)
        float perspVbottom;
        // Perspective top value in pixels (OpenGL)
        float perspVtop;
        // Z near value in pixels (OpenGL)
        float near;
        // Z far value in pixels (OpenGL)
        float far;
        // Camera's rotating angle value (OpenGL)
        float rotAngle;
        // Percentage of the rotAngle in X
        float rotX;
        // Percentage of the rotAngle in Y
        float rotY;
        // Percentage of the rotAngle in Z
        float rotZ;
        // Scene translation value for x
        float transX;
        // Scene translation value for y
        float transY;
        // Scene translation value for z
        float transZ;

        // PRP x value
        float prpX;
        // PRP y value
        float prpY;
        // PRP z value
        float prpZ;

        // Variable to set the 3D or 2D view of the framework instance
        View view;
    public:
        // Ambient K value
        float Ka;
        // Intensity of ambient light
        IRGB Ia;
        // Difuse K value
        float Kd;
        // Difuse of light
        IRGB Id;
        // Position of light source in X
        float lightX;
        // Position of light source in y
        float lightY;
        // Position of light source in z
        float lightZ;
        // Specular K value
        float Ks;
        // Specular intensity
        IRGB Is;
        // n value for specular
        int n;

        // Time variable for the timer function of OpenGL
        unsigned int time;

        // Empty GLInterface constructor
        GLInterface();
        // Custom window values for GLInterface constructor
        GLInterface(int wndWith, int wndHeight, int wndPosX, int wndPosY, const char* wndName);

        // Main display function of OpenGL framework
        static void displayWrapper();
        // Instance wrapper for the glutIdleFunc
        static void runWrapper(int timerValue);
        // Keyboard method wrapper
        static void keyboardWrapper(unsigned char key, int x, int y);

        // Set the 2D window's viewing values
        void setView2D(GLenum matrixMode,
                       float orthoVleft, float orthoVright, float orthoVbottom, float orthoVtop);
        // Set the 3D window's viewing values
        void setView3D(GLenum matrixMode,
                       float orthoVleft, float orthoVright, float orthoVbottom, float orthoVtop,
                       float near, float far, float rotAngle, float rotX, float rotY, float rotZ);

        // Set the 3D perspective view
        void setPerspective(GLenum matrixMode,
                            float perspVleft, float perspVright, float perspVbottom, float perspVtop,
                            float near, float far);

        // Set the background color
        void setBackgroundColor(float red, float green, float blue);

        // Set the translation values
        void setTranslationValues(float transX, float tranxY, float transZ);

        // Set the ilumination values
        void setIluminationValues(float Ka, float Iar, float Iag, float Iab, float Kd, float Idr, float Idg, float Idb, float lightX, float lightY, float lightZ);

        // Initialize the window's view
        void initView (void);
        // Starts the main framework
        void startFramework(int argc, char *argv[]);

        // Run app
        virtual void run();
        // Display function
        virtual void display(void);
        // Keyboard method
        virtual void keyboard(unsigned char key, int x, int y);

        // Set instance to GLInterface
        static void setInstance(GLInterface* framework);

        // Get an array with the prp coordinates
        float *getPRP();
        // Setter for the prp coordinates
        void setPRP(float x, float y, float z);
};

// This class makes every object drawable
class Drawable : public GLInterface{
    protected:
        // Brush to define the color, pointSize and lineStyle of the drawable object
        Brush brush;
    public:
        // Default constructor
        Drawable();
        // Constructor with custom values
        Drawable(int red, int green, int blue, float pointSize, GLenum lineStyle);
        // Draw method, the extended class needs to define its implementation
        virtual void draw();
        // Sets a custom brush
        void setBrush(int red, int green, int blue, float pointSize, GLenum lineStyle);
};