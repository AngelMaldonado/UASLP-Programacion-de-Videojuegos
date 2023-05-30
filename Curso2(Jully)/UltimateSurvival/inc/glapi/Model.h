#pragma once

#include <glapi/GLApi.h>

class Model
{
public:
	std::vector<Mesh> meshes;
	const std::string path;
	const bool gammaCorrection;

	Model(std::string const& path, bool gamma = false);
	void Draw(Shader& shader);

private:
	void LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);
	std::vector<Texture> loadedTextures;

	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadTextures(aiMaterial* material, aiTextureType type);
};

