#pragma once

class VertexBufferObject
{
private:
	/**
	 * @brief Id of the data buffer
	 */
	unsigned int idDataBuffer;
public:
	VertexBufferObject() : idDataBuffer(0) {}
	/**
	 * @brief VertexBuffer constructor
	 * @param buffer Pointer to the data itself that will be rendered/binded
	 * @param size Size of the buffer (how many elements)
	 * @return VertexBufferObject instance
	 */
	VertexBufferObject(const void* buffer, unsigned int size);

	/**
	 * @brief Destructor of VertexBufferObject instance, it deletes the buffer with idDataBuffer value
	 */
	~VertexBufferObject() {}

	void Dispose();
	
	/**
	 * @brief Function to bind the buffer
	 */
	void Bind() const; // <- const is added to prevent modifying idDataBuffer
	
	/**
	 * @brief Function to unbind the buffer
	 */
	void Unbind() const; // <- const is added to prevent modifying idDataBuffer
};

