//
// Created by edgar on 10/1/2023.
//

#include "BasicMesh.h"
BasicMesh::BasicMesh()
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

void BasicMesh::CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int vertexCount, unsigned int indexCount)
{
	this->indexCount = (GLsizei) indexCount;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr) (sizeof(indices[0]) * indexCount), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) (sizeof(vertices[0]) * vertexCount), vertices, GL_STATIC_DRAW);
	// Geometria
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, nullptr);
	glEnableVertexAttribArray(0);
	// Texturizado
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void *) (sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);
	// Normales
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void *) (sizeof(vertices[0]) * 5));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void BasicMesh::RenderMesh()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void BasicMesh::ClearMesh()
{
	if (IBO != 0)
	{
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}

	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	indexCount = 0;
}

BasicMesh::~BasicMesh()
{
	ClearMesh();
}
