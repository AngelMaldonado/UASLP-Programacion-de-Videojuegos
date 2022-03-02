//
// Created by Angel Maldonado on 3/3/2021.
//
#include "../../include/geometry.h"

vector<int> Mesh::getFaceVerticesIndices() {
    vector<int> faceVerticesIndices;
    for(Face f : faces) {
        for(Vertex v : f.getVertices()) {
            faceVerticesIndices.push_back(v.getIndex());
        }
    }
    return faceVerticesIndices;
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, getFaceVerticesIndices().size() * sizeof(unsigned int), &verticesIndices[0], GL_STATIC_DRAW);

    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // Mas atributos...

    glBindVertexArray(0);
}

Mesh::Mesh(string name, int lastIndex) {
    vertexIndex = lastIndex;
    this->name = name;
    getFaceVerticesIndices();
    setupMesh();
}

Mesh::Mesh(string name) {
    vertexIndex = 0;
    this->name = name;
    getFaceVerticesIndices();
    setupMesh();
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
