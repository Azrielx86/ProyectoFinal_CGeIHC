#pragma once

#include "GlobalConstants.h"
#include "Shader.h"
#include "model/Mesh.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
class Skybox
{
  public:
	Skybox();
	Skybox(std::vector<std::string> faceLocations);
	void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

	~Skybox();

  private:
	Model::Mesh *skyMesh;
	Shader *skyShader;
	GLuint textureId;
	GLuint uniformProjection, uniformView;
};
