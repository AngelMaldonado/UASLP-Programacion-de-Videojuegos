#pragma once

#include <glapi/GLApi.h>

#define GLAPI_MAX_BONE_INFLUENCE 4



/**
 *	Structure that defines vertex data
 */
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texCoords;
};

/**
 *	Structure that defines texture data
 */
struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;
};

/**
 *	Structure that defines material data
 */
struct Material {
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 ambient;
	float shininess;
};

const float CUBE_BUFFER[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f,  1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,  1.0f,
};

const unsigned int CUBE_INDICES[]{
	0, 1, 3, 3, 2, 1,
	1, 2, 5, 5, 2, 6,
	6, 7, 5, 5, 7, 4,
	4, 7, 3, 3, 4, 0,
	0, 1, 4, 4, 1, 5,
	3, 2, 7, 7, 2, 6
};

const unsigned int CUBE_NUM_INDICES = 36;

const unsigned int CUBE_BUFFER_SIZE = 8 * 11 * sizeof(float);

class Mesh {
public:
	
	const std::vector<Vertex> vertices;
	const std::vector<unsigned int> indices;
	const std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
		: vertices(vertices), indices(indices), textures(textures)
	{
		meshBufferLayout.Push<float>(3);	// Vertex coords
		meshBufferLayout.Push<float>(3);	// Vertex normals
		meshBufferLayout.Push<float>(3);	// Vertex color
		meshBufferLayout.Push<float>(2);	// Vertex texture coords
	}
	inline const VertexBufferLayout GetMeshBufferLayout() const& { return meshBufferLayout; }
private:
	VertexBufferLayout meshBufferLayout;
};

