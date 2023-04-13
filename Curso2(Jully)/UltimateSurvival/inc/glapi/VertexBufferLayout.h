#pragma once

#include <glapi/GLApi.h>

/**
 * @brief VertexBufferElement helps to store an element inside an instance of the VertexBufferLayout, it
 * represents a single data point (position, tex-coordinates, color, etc) inside the whole buffer layout
 * @param type Stores the data type of the element (GL_FLOAT, GL_UNSIGNED_INT, GL_UNSIGNED_BYTE, etc)
 * @param count Number of elements (3 coords xyz, 2 tex-coords xy, 3 rgb values, etc)
 * @param normalized It specifies if the values have to be normalized when the AttribPointer is specified
 */
struct VertexBufferElement
{
	/**
	 * Data type of the elements (GL_FLOAT, GL_UNSIGNED_INT, GL_UNSIGNED_BYTE)
	 */
	unsigned int type;

	/**
	 * Number of elements in the buffer
	 */
	unsigned int count;
	
	/**
	 * If the value(s) have to be normalized
	 */
	unsigned char normalized;

	/**
	 * Function to get the type size
	 */
	static unsigned int GetTypeSize(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:			return sizeof(float);
			case GL_UNSIGNED_INT:	return sizeof(unsigned int);
			case GL_UNSIGNED_BYTE:	return sizeof(unsigned char);
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout {
private:
	/**
	 * @brief Vector with VertexBufferElements which specifies data type and amount of each data-point
	 * for the buffer's layout.
	 */
	std::vector<VertexBufferElement> elements;

	/**
	 * @brief Length in bytes of the layout
	 */
	unsigned int stride;
public:
	/**
	 * @brief Default VertexBufferLayout constructor, it initializes stride at 0
	 */
	VertexBufferLayout() : stride(0) {}

	/**
	 * Template to push datatypes to the vector of elements
	 */
	template<typename T>
	void Push(unsigned int count) { ASSERT(false); }

	/**
	 * Adds `count` floats to the elements vector and sets stride += sizeof(GL_FLOAT) 
	 */
	template<>
	void Push<float>(unsigned int count)
	{
		elements.push_back({GL_FLOAT, count, GL_FALSE});
		stride += count * VertexBufferElement::GetTypeSize(GL_FLOAT);
	}
	
	/**
	 * Adds `count` unsigned int to the elements vector and sets stride += sizeof(GL_UNSIGNED_INT) 
	 */
	template<>
	void Push<unsigned int>(unsigned int count)
	{
		elements.push_back({GL_FLOAT, count, GL_FALSE});
		stride += count * VertexBufferElement::GetTypeSize(GL_UNSIGNED_INT);
	}
	
	/**
	 * Adds `count` unsigned byte to the elements vector and sets stride += sizeof(GL_UNSIGNED_BYTE) 
	 */
	template<>
	void Push<unsigned char>(unsigned int count)
	{
		elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
		stride += count * VertexBufferElement::GetTypeSize(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement> GetElements() const& { return elements; }
	inline unsigned int GetStride() const { return stride; }
};

