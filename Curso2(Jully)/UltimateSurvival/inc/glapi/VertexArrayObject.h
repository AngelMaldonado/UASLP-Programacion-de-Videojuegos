#pragma once

#include <glapi/GLApi.h>

class VertexArrayObject
{
private:
	/**
	 * @brief Id of the array buffer
	 */
	unsigned int idArrayBuffer;
public:
	/**
	 * @brief VertexArray constructor, creates VAO and assigns and id
	 * @return VertexArrayObject instance
	 */
	VertexArrayObject(bool generateArrayBuffer = false);
	
	/**
	 * @brief Destructor of VertexArrayObject instance, it deletes the buffer with idArrayBuffer value
	 */
	~VertexArrayObject() {}

	void Dispose();

	/**
	 * @brief Function to add a buffer (data) to the VertexArrayObject for later bindings and renderings
	 * @param buffer Buffer with the data (positions, tex-coords, colors, etc)
	 * @param layout Layout of the data (amount of data, stride, etc)
	 */
	void AddBuffer(const VertexBufferObject& buffer, const VertexBufferLayout& layout);
	
	/**
	 * @brief Function to bind the buffer
	 */
	void Bind() const; // <- const to make attributes read-only
	
	/**
	 * @brief Function to unbind the buffer
	 */
	void Unbind() const; // <- const to make attributes read-only
};
 
