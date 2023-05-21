#include <glapi/GLApi.h>

void Renderer::Dispose()
{
	vao.Dispose();
	vbo.Dispose();
	ebo.Dispose();
}

void Renderer::ClearBuffer() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::SetBufferObjects(VertexArrayObject vao, VertexBufferObject vbo, ElementBufferObject ebo)
{
	this->vao = vao;
	this->vbo = vbo;
	this->ebo = ebo;

	this->vao.Unbind();
	this->vbo.Unbind();
	this->ebo.Unbind();
}

void Renderer::Render() const
{
	vao.Bind();
	ebo.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ebo.GetNumIndices(), GL_UNSIGNED_INT, 0));
}
