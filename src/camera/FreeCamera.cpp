//
// Created by edgar on 11/26/2023.
//

#include "FreeCamera.h"

namespace Camera
{
FreeCamera::FreeCamera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;
}

void FreeCamera::Update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

glm::vec3 FreeCamera::getCameraPosition() { return this->position; }

glm::vec3 FreeCamera::getCameraDirection() { return glm::normalize(front); }

glm::mat4 FreeCamera::calculateViewMatrix() { return glm::lookAt(position, position + front, up); }

void FreeCamera::KeyControl(Input::KeyboardInput &keyboard)
{
	float velocity = moveSpeed * deltaTime;

	if (keyboard.getCurrentKeymap()->at(GLFW_KEY_W).pressed)
		position += front * velocity;
	if (keyboard.getCurrentKeymap()->at(GLFW_KEY_S).pressed)
		position -= front * velocity;
	if (keyboard.getCurrentKeymap()->at(GLFW_KEY_A).pressed)
		position -= right * velocity;
	if (keyboard.getCurrentKeymap()->at(GLFW_KEY_D).pressed)
		position += right * velocity;
}

void FreeCamera::MouseControl(Input::MouseInput &mouse)
{
	float xChange = mouse.getXChange() * turnSpeed;
	float yChange = mouse.getYChange() * turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	Update();
}
} // namespace Camera