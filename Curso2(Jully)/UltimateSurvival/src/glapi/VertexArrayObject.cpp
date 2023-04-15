#include <glapi/GLApi.h>

VertexArrayObject::VertexArrayObject(bool generateArrayBuffer)
	: idArrayBuffer(0)
{
	if (generateArrayBuffer)
	{
		GLCall(glGenVertexArrays(1, &idArrayBuffer));
		GLCall(glBindVertexArray(idArrayBuffer));
	}
}

void VertexArrayObject::Dispose()
{
	 GLCall(glDeleteVertexArrays(1, &idArrayBuffer));
}

void VertexArrayObject::AddBuffer(const VertexBufferObject& buffer, const VertexBufferLayout& layout)
{
	Bind();
	buffer.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)(intptr_t)offset);
		offset += element.count * VertexBufferElement::GetTypeSize(element.type);
	}
}

void VertexArrayObject::Bind() const
{
	GLCall(glBindVertexArray(idArrayBuffer));
}

void VertexArrayObject::Unbind() const
{
	GLCall(glBindVertexArray(0));
}

