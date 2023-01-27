#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

class Object3D 
{
    public:
        int NumberOfVertices;

        GLuint ShaderProgramID;
        GLuint VertexBufferID;
        GLuint VertexArrayID;

        mat4 ModelMatrix;
    
    public:
        mat4 Move(GLfloat x, GLfloat y, GLfloat z, GLfloat pitch, GLfloat yaw, GLfloat roll);
};