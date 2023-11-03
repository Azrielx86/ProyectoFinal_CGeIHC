//
// Created by edgar on 10/5/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_MESH_H
#define PROYECTOFINAL_CGEIHC_MESH_H

#include <glm/glm.hpp>
#include <string>
#include <vector>

class Mesh
{
  public:
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	struct Texture
	{
		unsigned int id;
		std::string type;
	};

	std::vector<Mesh::Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Mesh::Texture> textures;
	
//	void Draw();
  private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};

#endif // PROYECTOFINAL_CGEIHC_MESH_H
