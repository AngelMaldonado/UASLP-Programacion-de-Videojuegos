//
// Created by Angel Maldonado on 3/3/2021.
//
#include "../../include/geometry.h"

Vertex::Vertex() {}

Vertex::Vertex(double x, double y, double z) {
    index = -1;
    this->x = x;
    this->y = y;
    this->z = z;
}

Vertex::Vertex(double x, double y, double z, int index) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->index = index;
}

void Vertex::showCoordinatesFormatted() {
    cout << fixed << index << " (" << x << ", " << y << ", " << z << ')' << "\n";
}

void Vertex::showCoordinates() {
    cout << fixed << x << ' ' << y << ' ' << z << '\n';
}

int Vertex::getIndex() {
    return index;
}

double Vertex::getX() {
    return x;
}


double Vertex::getY() {
    return y;
}


double Vertex::getZ() {
    return z;
}

void Vertex::setX(double x) {
    this->x = x;
}

void Vertex::setY(double y) {
    this->y = y;
}

void Vertex::setZ(double z) {
    this->z = z;
}

double * Vertex::getCoordinates() {
    double* coordinates = new double[3];
    coordinates[0] = x; coordinates[1] = y; coordinates[2] = z;
    return coordinates;
}