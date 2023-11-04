//
// Created by edgar on 10/5/2023.
//

#include "Mesh.h"
namespace Model
{
void Mesh::createMesh(GLfloat *vtx, uint *idx, uint vtxCount, uint idxCount)
{
	this->idxCount = idxCount;

	// Buffer generation
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ARR_SIZE(idx, idxCount), idx, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, ARR_SIZE(vtx, vtxCount), vtx, GL_STATIC_DRAW);

	// Geometry attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vtx[0]) * 8, 0);
	glEnableVertexAttribArray(0);

	// Texture attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vtx[0]) * 8, (void *) (sizeof(vtx[0]) * 3));
	glEnableVertexAttribArray(1);

	// Normals
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vtx[0]) * 8, (void *) (sizeof(vtx[0]) * 5));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Mesh::render()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, idxCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::clear()
{
	if (IBO != 0)
		glDeleteBuffers(1, &IBO);
	if (VBO != 0)
		glDeleteBuffers(1, &VBO);
	if (VAO != 0)
		glDeleteBuffers(1, &VAO);

	VAO = VBO = IBO = 0;
	idxCount = 0;
}
Mesh::~Mesh()
{
	clear();
}

} // namespace Model
