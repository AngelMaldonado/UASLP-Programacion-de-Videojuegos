#include "../include/Object3D.h"

mat4 Object3D::Move(GLfloat x, GLfloat y, GLfloat z, GLfloat pitch, GLfloat yaw, GLfloat roll)
{
    mat4 matRoll, matPitch, matYaw;

    matRoll = rotate(mat4(1), roll, vec3(0.0, 0.0, 1.0));
    matPitch = rotate(mat4(1), pitch, vec3(1.0, 0.0, 0.0));
    matYaw = rotate(mat4(1), yaw, vec3(0.0, 1.0, 0.0));

    mat4 rotate = matRoll * matPitch * matYaw;

    ModelMatrix = translate(mat4(1), vec3(x, y, z)) * rotate;

    return ModelMatrix;
}