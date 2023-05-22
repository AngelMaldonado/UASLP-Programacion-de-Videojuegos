#include <glapi/GLApi.h>

Model::Model(std::string const& path, bool gamma = false)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	std::vector<Texture> textures;
	aiMaterial* material;
	aiMesh* mesh;
	std::vector<Texture> diffuse, specular, normal, height;
	for (unsigned int index = 0; index < node->mNumMeshes; index++)
	{
		mesh = scene->mMeshes[node->mMeshes[index]];
		material = scene->mMaterials[mesh->mMaterialIndex];
        diffuse = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuse.begin(), diffuse.end());
        specular = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specular.begin(), specular.end());
        normal = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normal.begin(), normal.end());
        height = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), height.begin(), height.end());
		meshes.push_back(
			Mesh(scene->mMeshes[node->mMeshes[index]], scene, textures)
		);
	}

	for (unsigned int index = 0; index < node->mNumChildren; index++)
		ProcessNode(node->mChildren[index], scene);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	bool skip = false;

	for (unsigned int index = 0; index < material->GetTextureCount(type); index++)
	{
		aiString string;
		material->GetTexture(type, index, &string);
		for (unsigned int indexLoaded = 0; indexLoaded < textures_loaded.size(); indexLoaded++)
			if (strcmp(textures_loaded[indexLoaded].path.data(), string.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[indexLoaded]);
				skip = true;
				break;
			}
		if (!skip)
		{
			Texture texture = Texture(string.C_Str());
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}

