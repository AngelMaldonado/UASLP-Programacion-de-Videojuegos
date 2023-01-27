// Fuente: https://github.com/BennyQBD/ModernOpenGLTutorial
#ifndef MESH_INCLUDED_H
#define MESH_INCLUDED_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "objloader.h"

using namespace glm;

struct Vertex
{
public:
	Vertex(const glm::vec3 &pos, const glm::vec2 &texCoord, const glm::vec3 &normal)
	{
		this->pos = pos;
		this->texCoord = texCoord;
		this->normal = normal;
	}

	glm::vec3 *GetPos() { return &pos; }
	glm::vec2 *GetTexCoord() { return &texCoord; }
	glm::vec3 *GetNormal() { return &normal; }

private:
	// x, y and z coordinates of the vertex
	glm::vec3 pos;
	glm::vec2 texCoord;
	glm::vec3 normal;
};

enum MeshBufferPositions
{
	POSITION_VB,
	TEXCOORD_VB,
	NORMAL_VB,
	INDEX_VB
};

class Mesh
{
public:
	mat4 ModelMatrix;
	unsigned int numVertices;
	Mesh(const std::string &fileName);
	Mesh(Vertex *vertices, unsigned int numVertices, unsigned int *indices, unsigned int numIndices);

	void Draw();

	virtual ~Mesh();

protected:
private:
	Mesh() {}
	static const unsigned int NUM_BUFFERS = 4;
	void operator=(const Mesh &mesh) {}

	void InitMesh(const IndexedModel &model);

	GLuint m_VertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	unsigned int m_numIndices;
};

#endif