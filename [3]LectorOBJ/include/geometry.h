/// Libraries
#ifndef std

using namespace std;

#endif
#pragma once
#include <iostream>
#include <memory>
#include "containers.h"
#include "glinterface.h"

// Coordinates: X, Y and Z
typedef struct {
    // X coordinate
    float x;
    // Y coordinate
    float y;
    // Z coordinate
    float z;
} Coordinates;

// Structure that defines a vector
typedef struct {
    Coordinates N;
    Coordinates origin;
} Vector;

/**
 * >> Vertex: is a point where two or more curves, lines, or edges meet.
 *
 * Vertex class: that contains x, y and z coordinates which describes the
 * position of the Vertex in the space.
 **/
class Vertex {
private:
    // Index to identify the vertex
    int index;
    // Coordinate in x.
    double x;
    // Coordinate in y.
    double y;
    // Coordinate in z.
    double z;

public:
    // Default constructor
    Vertex();

    // Constructor for Vertex with user-defined coordinates.
    Vertex(double x, double y, double z);

    // Constructor for Vertex with user-defined coordinates and index/id
    Vertex(double x, double y, double z, int index);

    // Shows the coordinates formatted with ',' and '()'
    void showCoordinatesFormatted();

    // Shows the coordinates x, y and z separated by a single space
    void showCoordinates();

    // Gets the number of vertex (minimum index is 0)
    int getIndex();

    // Returns the x coorinate
    double getX();

    // Returns the y coordinate
    double getY();

    //Returns the z coordinate
    double getZ();

    // Set the x coordinate
    void setX(double x);

    // Set the y coordinate
    void setY(double y);

    // Set the z coordinate
    void setZ(double);

    // Returns an array with the coordinates
    double * getCoordinates();
};

/**
 * >> Face: is a flat surface that forms part of the boundary of a solid object (mesh).
 *
 * Face class: contains a vector of Vertex objects which describes each vertex of the
 * face, then the position of the face in space.
 **/
class Face : public Drawable{
private:
    // Vector of Vertex objects
    vector <int> verticesIndices;
    // Vector of the actual vertices
    vector <Vertex> vertices;
    // Normal vector of the face
    Vertex normal;
    // Brush to draw de face
    Brush brush;
public:
    // Default constructor
    Face();
    // Constructor for a drawable face
    Face(int red, int green, int blue, float pointSize, GLenum lineStyle, vector<Vertex> faceVertices);
    // Constructor for the Face with a vector of Vertices
    Face(vector<int> verticesIndices, vector<Vertex> faceVertices);

    // Shows the stored Face with simple format
    void showFaceFormatted();

    // Shows the Vertices that the Faces are made of
    void showFace();

    // Returns the vector of the vertices indices
    vector<int> getVerticesIndices();

    // Returns the vertices of the face
    vector<Vertex> &getVertices();

    // Compute the normal vector of the face with the current face vertices
    void computeNormalVector();

    // Getter for the normal vector
    Vertex getNormalVector();

    // Getter for the normal as unitary vector
    Vertex getNormalVectorAsUnitary();

    // Specifies if the face is visible based on a PRP value
    bool visible(Coordinates PRP);

    virtual void draw();
};

/**
 * >> Mesh: a polygon mesh is a collection of vertices, edges and faces that defines the shape of a
 * >>       polyhedral object.
 *
 * Mesh class: it has only a list of Face objects, since Face class has a vector of Vertex objects
 * it's not necessary to make another list of those in this class. Mesh can interact with Vertex's and
 * Face's objects through their methods.
 **/
class Mesh {
private:
    string name;
    vector <Face> faces;
    int vertexIndex;
    vector <Vertex> vertices;
public:
    // If there is more than one Mesh, last index of past Mesh is required
    Mesh(string name, int lastIndex);

    // New mesh
    Mesh(string name);

    // Add a new Vertex to the Mesh passing a vector with its coordinates
    bool addVertex(vector<double> coordinates);
    
    // Add a new Face to the Mesh passing a vector with its vertices
    bool addFace(vector<int> vertices);

    Face getFace(int index);

    // Returns the list of Faces that compose the Mesh
    vector<Face> getFaces();

    // Returns the last Face added to the Mesh
    Face getLastFace();

    // Returns the vector of Vertex that compose the Mesh
    vector<Vertex> &getVertices();

    // Update the vertices of the faces with the current values of the Mesh
    void updateFaceVertices();

    // Returns the last Vertex added to the Mesh
    Vertex getLastVertex();

    // Returns certain Vertex by index
    Vertex getVertex(int vertexIndex);

    // Returns a pointer to the 3 coordinates that makes the face vertex
    double *getVertexAsArray(int vertexIndex);

    // Modifies the coordinates of a Vertex
    void setVertexCoordinates(double x, double y, double z, int vertexIndex);

    // Returns the Mesh name
    string getMeshName();

    // Returns the Vertex index of the vector
    int getVertexIndex();
};

/**
 * Object class
 **/
class Object : public Drawable{
    private:
        // Vector of meshes
        vector<Mesh> meshes;
        // Origin of the object
        Coordinates origin;
        // Boolean to make the draw method to draw or not the origin point
        bool drawOrigin;
    public:
        // Default constructor
        Object();
        // Constructor with a predefined vector of meshes
        Object(vector<Mesh> mesh);
        // Getter for the vector of meshes
        vector<Mesh> &getMeshes();
        // Getter for the object's origin
        Coordinates &getOrigin();
        // Setter for the object's origin
        void setOrigin(float x, float y, float z);
        // Show the origin point
        void showOrigin();
        // Hide the origin point
        void hideOrigin();
        virtual void draw();
};