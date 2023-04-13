#include <glapi/GLApi.h>

VertexBufferObject::VertexBufferObject(const void* buffer, unsigned int size)
{
	GLCall(glGenBuffers(1, &idDataBuffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, idDataBuffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW));
}

VertexBufferObject::~VertexBufferObject()
{
	//GLCall(glDeleteBuffers(1, &idDataBuffer));
}

void VertexBufferObject::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, idDataBuffer));
}

void VertexBufferObject::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

