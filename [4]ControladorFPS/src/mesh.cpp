// Fuente: https://github.com/BennyQBD/ModernOpenGLTutorial
#include "../include/mesh.h"
#include "../include/util.h"
#include "../include/debugTimer.h"
#include <map>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdlib.h>

Mesh::Mesh(const std::string& fileName)
{
	// indexedModel = OBJModel(fileName).ToIndexedModel();
	// printf("%d, %d, %d\n", (int)indexedModel.positions.size(), (int)indexedModel.texCoords.size(), (int)indexedModel.normals.size());
    // InitMesh(indexedModel);
	InitMesh(OBJModel(fileName).ToIndexedModel());
}

void Mesh::InitMesh(const IndexedModel& model)
{
	// Inicializa el modelo indexado del mesh
	// this->indexedModel = model;

	// std::vector<GLfloat> data = indexedModel.GetDataVector();

	ModelMatrix = mat4(1.0f);

    m_numIndices = model.indices.size();

	// VAO
    glGenVertexArrays(1, &m_VertexArrayObject);
	glBindVertexArray(m_VertexArrayObject);

		// Prepara el buffer genera 4 buffers
		glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);

		// VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(model.positions[0]) * model.positions.size(), &model.positions[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // index, num attributes, type, normalize, stride, offset 

		// UVs
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(model.texCoords[0]) * model.texCoords.size(), &model.texCoords[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		// Normales
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// Indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(model.indices[0]) * model.indices.size(), &model.indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

Mesh::Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
    IndexedModel model;

	for(unsigned int i = 0; i < numVertices; i++)
	{
		model.positions.push_back(*vertices[i].GetPos());
		model.texCoords.push_back(*vertices[i].GetTexCoord());
		model.normals.push_back(*vertices[i].GetNormal());
	}
	
	for(unsigned int i = 0; i < numIndices; i++)
        model.indices.push_back(indices[i]);

    InitMesh(model);
}

Mesh::~Mesh()
{
	// glDeleteBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	// glDeleteVertexArrays(1, &m_VertexArrayObject);
}

void Mesh::Draw()
{
	// glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferObject );

	// 	// Vertex Buffer Object
	// 	glBindBuffer( GL_ARRAY_BUFFER, m_VertexBufferObject );

	// 	int stride = 8 * sizeof(GLfloat); // size of one record in bytes: 8 * float [ Vx, Vy, Vz, Nx, Ny, Nz, Tv, Tu]
	// 	void* offsV  = (void *)(0 * sizeof(GLfloat)); // offset of the vertex inside the reccord
	// 	void* offsTC = (void *)(3 * sizeof(GLfloat)); // offset of the texture coordinate inside the reccord
	// 	void* offsNV = (void *)(6 * sizeof(GLfloat)); // offset of the normal vector inside the reccord

	// 	// Posiciones
	// 	glEnableVertexAttribArray( 0 );
	// 		glVertexAttribPointer( 0,  3, GL_FLOAT, GL_FALSE, stride, offsV );  // 3: Vx, Vy, Vz
	// 	// Texturas
	// 	glEnableVertexAttribArray( 1 );
	// 		glVertexAttribPointer( 1,  2, GL_FLOAT, GL_FALSE, stride, offsTC ); // 2: Tu, Tv 
	// 	// Normales
	// 	glEnableVertexAttribArray( 2 );
	// 	glVertexAttribPointer( 2, 3, GL_FLOAT, GL_TRUE,  stride, offsNV ); // 3: Nx, Ny, Nz  -  GL_TRUE: values should be normalized

	// 	glDrawElements( GL_TRIANGLES, (GLsizei)indexedModel.indices.size(), GL_UNSIGNED_INT, nullptr );

	// 	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	// 	glDisableVertexAttribArray( 0 );
	// 	glDisableVertexAttribArray( 1 );
	// 	glDisableVertexAttribArray( 2 );
	
	// glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 ); 

	glBindVertexArray(m_VertexArrayObject);

		glDrawElementsBaseVertex(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0, 0);
	
	glBindVertexArray(0);
}