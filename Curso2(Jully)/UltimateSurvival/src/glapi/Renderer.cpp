#include <glapi/GLApi.h>

Renderer::~Renderer()
{
	//vao.~VertexArrayObject();
	//vbo.~VertexBufferObject();
}

void Renderer::ClearBuffer() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::SetBufferObjects(VertexArrayObject vao, VertexBufferObject vbo, ElementBufferObject ebo)
{
	this->vao = vao;
	this->vbo = vbo;
	this->ebo = ebo;
}

void Renderer::Render() const
{
	vao.Bind();
	ebo.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ebo.GetNumIndices(), GL_UNSIGNED_INT, 0));
}

