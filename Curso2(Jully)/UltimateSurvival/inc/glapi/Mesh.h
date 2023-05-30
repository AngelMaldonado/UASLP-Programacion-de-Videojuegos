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
	glm::vec3 tangent;
	glm::vec3 bitangent;
	glm::vec3 color;
	glm::vec2 texCoords;
	float useDiffuseTexture;
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
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	~Mesh() {}

	inline const VertexBufferLayout GetMeshBufferLayout() const& { return meshBufferLayout; }
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	Material material;

	VertexBufferLayout meshBufferLayout;
	Material LoadMaterial(aiMaterial* material);
};

