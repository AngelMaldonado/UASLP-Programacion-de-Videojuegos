#include "../../include/geometry.h"

Object::Object(){}

Object::Object(vector<Mesh> meshes) {
    this->meshes = meshes;

    // Find a way to compute the center of the object to the fine the object's origin
    origin.x = origin.y = origin.z = 0;
    
    drawOrigin = true;
}

vector<Mesh> &Object::getMeshes() {
    return meshes;
}

int Object::countVertices() {
    int vertices = 0;
    for(Mesh m : meshes) {
        vertices += m.getVertices().size();
    }
    return vertices;
}

int Object::countTotalVertices() {
    int vertices = 0;
    for(Mesh m : meshes)
        for(Face f : m.getFaces())
            vertices += f.getVertices().size();
    return vertices;
}

float* Object::verticesAsArray() {
    float* vertices = new float(countTotalVertices());
    int vi = 0;

    for(Mesh m : meshes)
        for(Face f : m.getFaces())
            for(Vertex v : f.getVertices()) {
                vertices[vi] = v.getX(), vi++;
                vertices[vi] = v.getY(), vi++;
                vertices[vi] = v.getZ(), vi++;
            }

    return vertices;
}

Coordinates &Object::getOrigin() {
    return origin;
}

void Object::setOrigin(float x, float y, float z) {
    origin.x = x;
    origin.y = y;
    origin.z = z;
}

void Object::showOrigin() {
    drawOrigin = true;
}

void Object::hideOrigin() {
    drawOrigin = false;
}