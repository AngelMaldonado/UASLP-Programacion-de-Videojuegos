#pragma once

#include <glapi/GLApi.h>

class ElementBufferObject {
private:
	/**
	 * @brief Id of the element buffer
	 */
	unsigned int idElementBuffer;
	
	/**
	 * @brief Total number of indices
	 */
	unsigned int numIndices;
public:
	ElementBufferObject() : idElementBuffer(0), numIndices(0) {}
	/**
	 * @brief Constructor of Element Buffer Object
	 * @param buffer Buffer where the indices are stored
	 * @param numIndices Amount of indices stored in the buffer
	 * @return Instance of Element Buffer Object
	 */
	ElementBufferObject(const unsigned int* buffer, unsigned int numIndices);
	~ElementBufferObject() {}
	void Dispose();

	void Bind() const;
	void Unbind() const;
	inline unsigned int GetNumIndices() const { return numIndices; }
};

