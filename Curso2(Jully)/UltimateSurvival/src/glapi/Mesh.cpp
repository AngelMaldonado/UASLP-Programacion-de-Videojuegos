#include <glapi/GLApi.h>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	: vertices(vertices), indices(indices), textures(textures)
{
	meshBufferLayout.Push<float>(3);	// Vertex coords
	meshBufferLayout.Push<float>(3);	// Vertex normals
	meshBufferLayout.Push<float>(3);	// Vertex color
	meshBufferLayout.Push<float>(2);	// Vertex texture coords
}

Mesh::Mesh(aiMesh* mesh, const aiScene* scene, std::vector<Texture> textures)
	: textures(textures)
{
	glm::vec3 vector3;
	glm::vec2 vector2;
	Vertex vertex;

	// Load Vertices
	for (unsigned int index = 0; index < mesh->mNumVertices; index++)
	{
		// POSITIONS
		vector3.x = mesh->mVertices[index].x;
		vector3.y = mesh->mVertices[index].y;
		vector3.z = mesh->mVertices[index].z;
		vertex.position = vector3;
		// NORMALS
		if (mesh->HasNormals())
		{
			vector3.x = mesh->mNormals[index].x;
			vector3.y = mesh->mNormals[index].y;
			vector3.z = mesh->mNormals[index].z;
			vertex.normal = vector3;
		}
		// TEX COORDS
		if (mesh->mTextureCoords[0])
		{
			vector2.x = mesh->mTextureCoords[0][index].x;
			vector2.y = mesh->mTextureCoords[0][index].y;
			vertex.texCoords = vector2;
			vector3.x = mesh->mTangents[index].x;
			vector3.y = mesh->mTangents[index].y;
			vector3.z = mesh->mTangents[index].z;
			vertex.tangent = vector3;
			vector3.x = mesh->mBitangents[index].x;
			vector3.y = mesh->mBitangents[index].y;
			vector3.z = mesh->mBitangents[index].z;
			vertex.bitangent = vector3;
		}
		else
			vertex.texCoords = glm::vec2(0.0f, 0.0f);

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
		vertices.push_back(vertex);
	}

	// Load Indices
	for (unsigned int index = 0; index < mesh->mNumFaces; index++)
	{
		aiFace face = mesh->mFaces[index];
		for (unsigned int vertexIndex = 0; vertexIndex < face.mNumIndices; vertexIndex++)
			indices.push_back(face.mIndices[vertexIndex]);
	}

	// Load Materials
	material = LoadMaterial(scene->mMaterials[mesh->mMaterialIndex]);

	// Prepare layout
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

