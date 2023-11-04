//
// Created by edgar on 10/1/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_BASICMESH_H
#define PROYECTOFINAL_CGEIHC_BASICMESH_H

#include <GL/glew.h>
namespace Model
{

class BasicMesh
{
  public:
	BasicMesh();
	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int vertexCount, unsigned int indexCount);
	void RenderMesh();
	void ClearMesh();
	virtual ~BasicMesh();

  private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
};
} // namespace Model
#endif // PROYECTOFINAL_CGEIHC_BASICMESH_H
