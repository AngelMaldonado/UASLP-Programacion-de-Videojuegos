/// Libraries
#ifndef std

using namespace std;

#endif
#pragma once
#include <iostream>
#include <fstream> // Library to work with files
#include <sstream>
#include <ctype.h>
#include "geometry.h"

/**
 * OBJFileReader class: this class allows the user to read a *.obj file and store the information from that file
 * to different data structures.
 * First, it is required to define a vector<Mesh> to be able to start reading the file, since Mesh class has as
 * attributes a list<Face> (list of faces) and a vector<Vertex> (vector of vertices), and the readFile() method
 * makes all the work to read each line of the .obj file and recognize the characters that identifies the objects (o),
 * vertices (v), and faces (f).
 **/
class OBJFileReader {
private:
    /*
     * This fstream object allows the program to search and, if it succeeded, manipulates the file
     * with I/O operations
     */
    fstream fileReader;
    // String to store the file's name
    string fileName;
    // Path of the file (if applies)
    string filePath;
public:
    // Method to read de obj file and store info in a vector of meshes
    vector<Mesh> readFile(string fileName, char printReading);

    // Returns a vector of Vertex objects that describe a Face in current line at reading process
    // Requires the current mesh which contains the continuity of vertices' indexes that were added
    // Requires the last vertex index that were stored in a previous mesh (if there is more than one mesh)
    vector<int> getFaceWithLine(string currentFileLine, Mesh currentMesh, int lastVrtxIndexInPrevMesh);

    // Returns a vector with the coordinates of the vertex in current line at reading process
    vector<double> getVertexCoordinatesWithLine(string currentFileLine);

    // This method returns the object's id that is in actual line at reading process
    string getMeshNameWithLine(string currentFileLine);

    // Print the read info that was stored in vector<Mesh> as the original .obj file
    void printInfoAsFile(vector<Mesh> meshes);

    // NEED TO CODE
    bool validateFile(string fileName);

    // Closes the file read
    void closeFile();
};