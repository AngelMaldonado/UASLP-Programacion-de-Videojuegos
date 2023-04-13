#include <glapi/GLApi.h>

ElementBufferObject::ElementBufferObject(const unsigned int* buffer, unsigned int numIndices)
	: numIndices(numIndices)
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	GLCall(glGenBuffers(1, &idElementBuffer));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idElementBuffer));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), buffer, GL_STATIC_DRAW));
}

ElementBufferObject::~ElementBufferObject()
{
	//GLCall(glDeleteBuffers(1, &idElementBuffer));
}

void ElementBufferObject::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idElementBuffer));
}

void ElementBufferObject::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

