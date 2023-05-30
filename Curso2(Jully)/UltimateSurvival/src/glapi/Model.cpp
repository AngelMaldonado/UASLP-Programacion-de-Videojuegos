#include <glapi/GLApi.h>

Model::Model(std::string const& path, bool gamma)
	: gammaCorrection(gamma)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Could not load model at " << path << ": " << importer.GetErrorString() << std::endl;
		return;
	}

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	aiMesh* mesh;
	std::vector<Texture> diffuse, specular, normal, height;
	for (unsigned int index = 0; index < node->mNumMeshes; index++)
	{
		mesh = scene->mMeshes[node->mMeshes[index]];
		/*
		material = scene->mMaterials[mesh->mMaterialIndex];
        diffuse = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuse.begin(), diffuse.end());
        specular = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specular.begin(), specular.end());
        normal = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normal.begin(), normal.end());
        height = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), height.begin(), height.end());
		*/
		meshes.push_back(ProcessMesh(scene->mMeshes[node->mMeshes[index]], scene));
	}

	for (unsigned int index = 0; index < node->mNumChildren; index++)
		ProcessNode(node->mChildren[index], scene);
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	// Total vertices that mesh has
	std::vector<Vertex> vertices;
	// Total textures that are loaded/included with the mesh
	std::vector<Texture> textures;
	// Total indices that mesh has
	std::vector<unsigned int> indices;
	// Diffuse maps textures
	std::vector<Texture> diffuseMaps;
	// Specular maps textures
	std::vector<Texture> specularMaps;
	// Current vertex
	Vertex vertex;
	// Current face
	aiFace face;
	// Current material
	aiMaterial* material;

	// Load Vertices
	for (unsigned int index = 0; index < mesh->mNumVertices; index++)
	{
		// Position
		vertex.position = glm::vec3(
			mesh->mVertices[index].x,
			mesh->mVertices[index].y,
			mesh->mVertices[index].z
		);

		// Normals
		if (mesh->HasNormals())
		{
			vertex.normal = glm::vec3(
				mesh->mNormals[index].x,
				mesh->mNormals[index].y,
				mesh->mNormals[index].z
			);
		}

		// Texture coords
		if (mesh->mTextureCoords[0])
		{
			vertex.texCoords = glm::vec2(
				mesh->mTextureCoords[0][index].x,
				mesh->mTextureCoords[0][index].y
			);
			/*
			vector3.x = mesh->mTangents[index].x;
			vector3.y = mesh->mTangents[index].y;
			vector3.z = mesh->mTangents[index].z;
			vertex.tangent = vector3;
			vector3.x = mesh->mBitangents[index].x;
			vector3.y = mesh->mBitangents[index].y;
			vector3.z = mesh->mBitangents[index].z;
			vertex.bitangent = vector3;*/
		}
		else
			vertex.texCoords = glm::vec2(0.0f);

		/*
		if (scene->mNumMaterials > mesh->mMaterialIndex)
		{
			const auto& material = scene->mMaterials[mesh->mMaterialIndex];
			aiColor4D diffuse;
			if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
				vertex.color = glm::vec4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
			if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
				vertex.useDiffuseTexture = 1.f;
			else
				vertex.useDiffuseTexture = 0.f;
		}
		*/
		vertices.push_back(vertex);
	}

	// Load indices
	for (unsigned int faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++)
	{
		face = mesh->mFaces[faceIndex];
		for (unsigned int vertexIndex = 0; vertexIndex < face.mNumIndices; vertexIndex++)
			indices.push_back(face.mIndices[vertexIndex]);
	}

	// Load materials
	if (mesh->mMaterialIndex >= 0)
	{
		material = scene->mMaterials[mesh->mMaterialIndex];

		// Diffuse maps
		diffuseMaps = LoadTextures(material, aiTextureType_DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		// Specular maps
		specularMaps = LoadTextures(material, aiTextureType_SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadTextures(aiMaterial* material, aiTextureType type)
{
	// Boolean to prevent duplicate texture loading
	bool skip;
	// String to get the path of the texture from material
	aiString texturePath;
	// Store the loaded textures
	std::vector<Texture> textures;

	for (unsigned int textureIndex = 0; textureIndex < material->GetTextureCount(type); textureIndex++)
	{
		skip = false;
		material->GetTexture(type, textureIndex, &texturePath);
		for(unsigned int loadedTextureIndex = 0; loadedTextureIndex < loadedTextures.size(); loadedTextureIndex++)
			if (std::strcmp(loadedTextures[loadedTextureIndex].path.data(), texturePath.C_Str()))
			{
				textures.push_back(loadedTextures[loadedTextureIndex]);
				skip = true;
				break;
			}
		if (!skip)
		{
			Texture texture(texturePath.C_Str(), type);
			textures.push_back(texture);
			loadedTextures.push_back(texture);
		}
	}

	return textures;
}

void Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName)
{
	/*
	std::vector<Texture> textures;
	bool skip = false;

	for (unsigned int index = 0; index < material->GetTextureCount(type); index++)
	{
		aiString string;
		material->GetTexture(type, index, &string);
		for (unsigned int indexLoaded = 0; indexLoaded < loadedTextures.size(); indexLoaded++)
			if (strcmp(loadedTextures[indexLoaded].path.data(), string.C_Str()) == 0)
			{
				textures.push_back(loadedTextures[indexLoaded]);
				skip = true;
				break;
			}
		if (!skip)
		{
			Texture texture = Texture(string.C_Str());
			textures.push_back(texture);
			loadedTextures.push_back(texture);
		}
	}
	return textures;
	return std::vector<Texture>();
	*/
}

