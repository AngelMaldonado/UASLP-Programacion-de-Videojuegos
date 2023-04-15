#pragma once

class Renderer {
public:
	Renderer() {}
	Renderer(VertexArrayObject vao, VertexBufferObject vbo, ElementBufferObject ebo)
		: vao(vao), vbo(vbo), ebo(ebo) {}

	~Renderer() {}

	/**
	 * @brief Function to clear the color buffer
	 */
	void ClearBuffer() const;

	void SetBufferObjects(VertexArrayObject vao, VertexBufferObject vbo, ElementBufferObject ebo);

	void Dispose();

	/**
	 * @brief Function to render current VertexArrayObject and VertexBufferObject
	 */
	void Render() const;
private:
	/**
	 * Vertex Array Object (VAO) global to make the batch rendering
	 */
	VertexArrayObject vao;

	/**
	 * Vertex Buffer Object (VBO) global to make the batch rendering
	 */
	VertexBufferObject vbo;

	/**
	 * Element Buffer Object (EBO) stores the indices and order in which vertices are drawn
	 */
	ElementBufferObject ebo;
};

