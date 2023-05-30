#include <glapi/GLApi.h>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	: vertices(vertices), indices(indices), textures(textures)
{
	meshBufferLayout.Push<float>(3);	// Vertex coords
	meshBufferLayout.Push<float>(3);	// Vertex normals
	meshBufferLayout.Push<float>(3);	// Vertex color
	meshBufferLayout.Push<float>(2);	// Vertex texture coords
}



Material Mesh::LoadMaterial(aiMaterial* material)
{
	aiColor3D color(0.0f, 0.0f, 0.0f);
	Material myMaterial;
	float shininess;

	material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	myMaterial.diffuse = glm::vec3(color.r, color.b, color.g);

	material->Get(AI_MATKEY_COLOR_AMBIENT, color);
	myMaterial.ambient = glm::vec3(color.r, color.b, color.g);

	material->Get(AI_MATKEY_COLOR_SPECULAR, color);
	myMaterial.specular = glm::vec3(color.r, color.b, color.g);

	material->Get(AI_MATKEY_SHININESS, shininess);
	myMaterial.shininess = shininess;

	return myMaterial;
}

