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