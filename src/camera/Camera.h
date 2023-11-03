//
// Created by edgar on 10/6/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_CAMERA_H
#define PROYECTOFINAL_CGEIHC_CAMERA_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Camera
{
class Camera
{
  public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	/*
	 * Manera del lab en el bucle principal;
	 * // Recibir eventos del usuario
	 * glfwPollEvents();
	 * camera.keyControl(mainWindow.getsKeys(), deltaTime);
	 * camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
	 * 
	 * Objetivo:
	 * Solo una función Update
	 */
	
	void keyControl(bool *keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);

	glm::vec3 getCameraPosition();
	glm::vec3 getCameraDirection();
	glm::mat4 calculateViewMatrix();

  private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	void update();
};
} // namespace Camera
#endif // PROYECTOFINAL_CGEIHC_CAMERA_H
