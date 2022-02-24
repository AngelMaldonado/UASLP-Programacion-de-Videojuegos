#include "../../include/transformations.h"

void TransformationsHandler::translateObject(Object &object, double dx, double dy, double dz) {
    int r, k;
    double T[4][4] = {1, 0, 0, (double)dx,
                      0, 1, 0, (double)dy,
                      0, 0, 1, (double)dz,
                      0, 0, 0, 1};
        
    for(Mesh &mesh : object.getMeshes()) {
        for(int point = 0; point < (int)mesh.getVertices().size(); point++) {
            Vertex vertex = mesh.getVertex(point);
            double coordinates[4] = {vertex.getCoordinates()[0], vertex.getCoordinates()[1], vertex.getCoordinates()[2], 1};
            double newCoordinates[4] = {0, 0, 0, 1};
            for(r = 0; r < 4; r++)
                for(k = 0; k < 4; k++)
                    newCoordinates[r] += T[r][k] * coordinates[k];
            mesh.setVertexCoordinates(newCoordinates[0], newCoordinates[1], newCoordinates[2], point);
        }
        mesh.updateFaceVertices();
    }
}

void TransformationsHandler::scaleObject(Object &object, double s) {
    int r, k;
    double S[4][4] = {s, 0, 0, 0,
                      0, s, 0, 0,
                      0, 0, s, 0,
                      0, 0, 0, 1};
    for(Mesh &mesh : object.getMeshes()) {
        for(int point = 0; point < (int)mesh.getVertices().size(); point++) {
            Vertex vertex = mesh.getVertex(point);
            double coordinates[4] = {vertex.getCoordinates()[0], vertex.getCoordinates()[1], vertex.getCoordinates()[2], 1};
            double newCoordinates[4] = {0, 0, 0, 1};
            for(r = 0; r < 4; r++)
                for(k = 0; k < 4; k++)
                    newCoordinates[r] += S[r][k] * coordinates[k];
            mesh.setVertexCoordinates(newCoordinates[0], newCoordinates[1], newCoordinates[2], point);
        }
        mesh.updateFaceVertices();
    }
}

void TransformationsHandler::rotateObject(Object &object, Coordinates pivot, double angleX, double angleY, double angleZ) {
    double rotMatrixX[4][4], rotMatrixY[4][4], rotMatrixZ[4][4];
    int r, c;
    for(r = 0; r < 4; r++)
        for(c = 0; c < 4; c++)
            rotMatrixX[r][c] = rotMatrixY[r][c] = rotMatrixZ[r][c] = 0;
    rotMatrixX[0][0] = rotMatrixX[3][3] = rotMatrixY[1][1] = rotMatrixY[3][3] = rotMatrixZ[2][2] = rotMatrixZ[3][3] = 1;
    // Rx(0)
    rotMatrixX[1][1] = cos(angleX * (M_PI/180));
    rotMatrixX[1][2] = -sin(angleX * (M_PI/180));
    rotMatrixX[2][1] = sin(angleX * (M_PI/180));
    rotMatrixX[2][2] = cos(angleX * (M_PI/180));
    // Ry(0)
    rotMatrixY[0][0] = cos(angleY * (M_PI/180));
    rotMatrixY[2][0] = -sin(angleY * (M_PI/180));
    rotMatrixY[0][2] = sin(angleY * (M_PI/180));
    rotMatrixY[2][2] = cos(angleY * (M_PI/180));
    // Rz(0)
    rotMatrixZ[0][0] = cos(angleZ * (M_PI/180));
    rotMatrixZ[0][1] = -sin(angleZ * (M_PI/180));
    rotMatrixZ[1][0] = sin(angleZ * (M_PI/180));
    rotMatrixZ[1][1] = cos(angleZ * (M_PI/180));

    translateObject(object, -pivot.x, -pivot.y, -pivot.z);

    // ROTATION X
    for(Mesh &mesh : object.getMeshes()) {
        for(int point = 0; point < (int)mesh.getVertices().size(); point++) {
            Vertex vertex = mesh.getVertex(point);
            double coordinates[4] = {vertex.getCoordinates()[0], vertex.getCoordinates()[1], vertex.getCoordinates()[2], 1};
            double newCoordinates[4] = {0, 0, 0, 1};
            for(r = 0; r < 4; r++)
                for(c = 0; c < 4; c++)
                    newCoordinates[r] += rotMatrixX[r][c] * coordinates[c];
            mesh.setVertexCoordinates(newCoordinates[0], newCoordinates[1], newCoordinates[2], point);
        }
    }

    // ROTATION Y
    for(Mesh &mesh : object.getMeshes()) {
        for(int point = 0; point < (int)mesh.getVertices().size(); point++) {
            Vertex vertex = mesh.getVertex(point);
            double coordinates[4] = {vertex.getCoordinates()[0], vertex.getCoordinates()[1], vertex.getCoordinates()[2], 1};
            double newCoordinates[4] = {0, 0, 0, 1};
            for(r = 0; r < 4; r++)
                for(c = 0; c < 4; c++)
                    newCoordinates[r] += rotMatrixY[r][c] * coordinates[c];
            mesh.setVertexCoordinates(newCoordinates[0], newCoordinates[1], newCoordinates[2], point);
        }
    }

    // ROTATION Z
    for(Mesh &mesh : object.getMeshes()) {
        for(int point = 0; point < (int)mesh.getVertices().size(); point++) {
            Vertex vertex = mesh.getVertex(point);
            double coordinates[4] = {vertex.getCoordinates()[0], vertex.getCoordinates()[1], vertex.getCoordinates()[2], 1};
            double newCoordinates[4] = {0, 0, 0, 1};
            for(r = 0; r < 4; r++)
                for(c = 0; c < 4; c++)
                    newCoordinates[r] += rotMatrixZ[r][c] * coordinates[c];
            mesh.setVertexCoordinates(newCoordinates[0], newCoordinates[1], newCoordinates[2], point);
        }
    }

    translateObject(object, pivot.x, pivot.y, pivot.z);
    for(Mesh &mesh : object.getMeshes())
        mesh.updateFaceVertices();
}