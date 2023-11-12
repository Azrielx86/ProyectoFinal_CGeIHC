//
// Created by edgar on 11/10/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_BONEMESH_H
#define PROYECTOFINAL_CGEIHC_BONEMESH_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#define MAX_BONE_INFLUENCE 8

namespace Model
{

using uint = unsigned int;

struct Vertex
{
	glm::vec3 Position;
	glm::vec2 TexCoords;
	glm::vec3 Normal;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	float m_Weights[MAX_BONE_INFLUENCE];
};

// struct Texture
//{
//	uint id;
//	std::string type;
// };

class BoneMesh
{
  public:
	BoneMesh(const std::vector<Vertex> &vertex, const std::vector<uint> &indices);
	void render();

  private:
	GLuint VAO, VBO, IBO;
	std::vector<Vertex> vertex;
	std::vector<uint> indices;
	void initMesh();
};

} // namespace Model

#endif // PROYECTOFINAL_CGEIHC_BONEMESH_H
