//
// Created by edgar on 10/6/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_CAMERA_H
#define PROYECTOFINAL_CGEIHC_CAMERA_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//
#include "../input/KeyboardInput.h"
#include "../input/MouseInput.h"

#ifdef DEBUG
#include <iostream>
#endif

namespace Camera
{
class Camera
{
  public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed, bool locked = false);

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

	void keyControl(Input::KeyboardInput &keyboard, GLfloat deltaTime);
	void mouseControl(Input::MouseInput &mouse);

	glm::vec3 getCameraPosition();
	glm::vec3 getCameraDirection();
	glm::mat4 calculateViewMatrix();

  private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

  public:
	GLfloat getYaw() const;
	GLfloat getPitch() const;

  private:
	GLfloat yaw;
	GLfloat pitch;

	GLfloat moveSpeed;
	GLfloat turnSpeed;
	
	bool locked;

	void update();
};
} // namespace Camera
#endif // PROYECTOFINAL_CGEIHC_CAMERA_H
