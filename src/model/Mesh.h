//
// Created by edgar on 10/5/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_MESH_H
#define PROYECTOFINAL_CGEIHC_MESH_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#define ARR_SIZE(array, count) (sizeof (array[0]) * count)

namespace Model
{
using uint = unsigned int;

class Mesh
{
  public:
	Mesh() = default;
	~Mesh();
	void createMesh(GLfloat *vtx, uint *idx, uint vtxCount, uint idxCount);
	void render();
	void clear();

	//	void Draw();
  private:
	uint VAO, VBO, IBO;
	GLsizei idxCount;
};

} // namespace Model
#endif // PROYECTOFINAL_CGEIHC_MESH_H
