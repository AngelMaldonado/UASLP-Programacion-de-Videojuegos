//
// Created by Angel Maldonado on 3/3/2021.
//
#include "../../include/geometry.h"

Mesh::Mesh(string name, int lastIndex) {
    vertexIndex = lastIndex;
    this->name = name;
}

Mesh::Mesh(string name) {
    vertexIndex = 0;
    this->name = name;
}

bool Mesh::addVertex(vector<double> coordinates) {
    if (!coordinates.empty()) {
        vertices.push_back(Vertex(coordinates[0],       // x
                                    coordinates[1],     // y
                                    coordinates[2],     // z
                                    vertexIndex++));
        return true;
    }
    return false;
}

bool Mesh::addFace(vector<int> verticesIndices) {
    // Validate the amount of vertices to make a face
    if (verticesIndices.size() >= 3) {
        vector<Vertex> faceVertices;
        for(int vertexIndex : verticesIndices)
            for(Vertex vertex : vertices)
                if(vertex.getIndex() == vertexIndex) {
                    faceVertices.push_back(vertex);
                    break;
                }

        // Store the face in the list of Face's objects
        faces.push_back(Face(verticesIndices, faceVertices));
        return true;
    } else {
        return false;
    }
}

Face Mesh::getFace(int index) {
    return faces.at(index);
}

vector<Face> Mesh::getFaces() {
    return faces;
}

void Mesh::updateFaceVertices() {
    for(Face &face : faces) {
        for(Vertex &faceVertex : face.getVertices())
            for(Vertex meshVertex : vertices)
                if(meshVertex.getIndex() == faceVertex.getIndex()) {
                    faceVertex.setX(meshVertex.getX());
                    faceVertex.setY(meshVertex.getY());
                    faceVertex.setZ(meshVertex.getZ());
                    break;
                }
        face.computeNormalVector();
    }
    
}

Face Mesh::getLastFace() {
    return faces.back();
}

vector<Vertex> &Mesh::getVertices() {
    return vertices;
}

Vertex Mesh::getLastVertex() {
    return vertices.back();
}

Vertex Mesh::getVertex(int vertexIndex) {
    return vertices.at(vertexIndex);
}

double *Mesh::getVertexAsArray(int vertexIndex) {
    double *coordinates = new double(3);

    coordinates[0] = vertices.at(vertexIndex).getX();
    coordinates[1] = vertices.at(vertexIndex).getY();
    coordinates[2] = vertices.at(vertexIndex).getZ();
    return coordinates;
}

void Mesh::setVertexCoordinates(double x, double y, double z, int vertexIndex) {
    vertices[vertexIndex].setX(x);
    vertices[vertexIndex].setY(y);
    vertices[vertexIndex].setZ(z);
}

string Mesh::getMeshName() {
    return name;
}

int Mesh::getVertexIndex() {
    return vertexIndex;
}
