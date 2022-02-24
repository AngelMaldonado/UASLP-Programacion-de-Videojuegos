#pragma once
#include <math.h>
#include <iostream>
#include "glinterface.h"
#include "geometry.h"

using namespace std;

/**
 * TrajectoryHandler class: this class allow the handling of linear and curve trajectories, when an instance of this
 * class is created, it can draw or move objects calling the specific trajectory method.
 **/
enum Algorithm {
            ALG_BRSNHM_DRAW_LINE = 1,
            ALG_HERMITE_DRAW_CURVE = 2
        };

// Object's directions
enum Direction {
    O_TOWARDS = 1,
    O_BACKWARDS
};

// Values for Bresenham algorithm
typedef struct {
    // Dx for Bresenham
    int dx, dx2;
    // Dy for Bresenham
    int dy, dy2;
    // Amount of steps for Bresenham
    int n;
    // Decision parameter 1 for Bresenham
    int pk;
    // Decision parameter 1 for Bresenham
    int pk2;
    // 2dy value for Bresenham
    int _2dy;
    // 2dx value for Bresenham
    int _2dx;
    // 2dz value for Bresenham
    int _2dz;
    // 2dy - dx value for Bresenham
    int _2dy_Minus_2dx;
    // 2dz - dx value for Bresenham
    int _2dz_Minus_dx;
    // 2dx - dy value for Bresenham
    int _2dx_Minus_2dy;
    // 2dz - dy value for Bresenham
    int _2dz_Minus_dy;
    // 2dy - dz value for Bresenham
    int _2dy_Minus_dz;
    // 2dx - dz value for Bresenham
    int _2dx_Minus_dz;
    // Slope value for Bresenham
    float m;
    // Point 1 for Bresenham
    Coordinates p1;
    // Point 2 for Bresenham
    Coordinates p2;
    // Brush to paint the line with Bresenham
    Brush brush;
    // Object's direction to animate
    Direction objectDirection;
} Bresenham;

// Values for Hermite algorithm
typedef struct {
    // Steps to draw the curve's points
    float increment;
    // Point 1 for Hermite
    Coordinates p1;
    // Vector 1 for Hermite
    Coordinates r1;
    // Point 2 for Hermite
    Coordinates p4;
    // Vector 2 for Hermite
    Coordinates r4;
    // T value to compute coordinates
    float t;
    // Brush to paint the curve with Hermite
    Brush brush;
    // Object's direction to animate
    Direction objectDirection;
} Hermite;

class TrajectoryHandler : public Drawable {
    private:
        // Selected algorithm/method
        Algorithm algorithm;

        // Handler for Bresenham's algorithm
        Bresenham bresenham;
        // Handler for Hermite's algorithm
        Hermite hermite;

        // Initialize the drawing line
        void initDrawingTrajectory(Brush brush);

        // Compute the slope of the given 2 pairs of coordinates
        float slope(float x1, float y1, float x2, float y2);

        // Compute the values for Bresenham algorithm (the points needs to be set before)
        void initBresenham();
    public:
        // Empty constructor
        TrajectoryHandler();

        // Sets the algorithm that GLInterface is going to execute
        void setAlgorithm(Algorithm algorithm);

        // Set the values for Bresenham
        void setBresenhamValues(float fx1, float fy1, float fx2, float fy2,
                                int red, int green, int blue, float pointSize, GLenum lineStyle);

        // Sets custom hermite curve values
        void setHermiteValues(float xP1, float yP1, float zP1,float xP4, float yP4, float zP4,
                              float xR1, float yR1, float zR1, float xR4, float yR4, float zR4,
                              float increment,
                              int red, int green, int blue, float pointSize, GLenum lineStyle);

        // Overrided draw function from Drawable class
        virtual void draw();

        // Draw a line following the Bresenham algorithm
        void bresenhamDrawLine();

        // Draw a curve following the Hermite algorithm
        void hermiteDrawCurve();

        // Animate an object, this function needs to be called in the run() definition
        void bresenhamAnimateObject(Object &object, Coordinates &origin);

        // Animate an object with a predefined Hermite curve trajectory
        void hermiteAnimateObject(Object &object, Coordinates &origin);
};

class TransformationsHandler {
    private:
        // Translation Matrix
        double translationMatrix[4][4];
        // Rotation Matrix for X axis
        double rotMatrixX[4][4];
        // Rotation Matrix for Y axis
        double rotMatrixY[4][4];
        // Rotation Matrix for Z axis
        double rotMatrixZ[4][4];
        // Matrix composition
        double matrixComposition[4][4];
    public:
        // Translate an object at T[dx, dy, dz, 1]
        static void translateObject(Object &object, double dx, double dy, double dz);
        // Scale an object
        static void scaleObject(Object &object, double scalingValue);
        // Rotate an object
        static void rotateObject(Object &object, Coordinates pivot, double rx, double ry, double rz);
};