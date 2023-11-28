#pragma once

#include <GL/glew.h>

class MeshPrimitive
{
public:
	MeshPrimitive();

	void CreateMeshPrimitive(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void RenderMeshPrimitive();
	void ClearMeshPrimitive();

	~MeshPrimitive();

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
};

