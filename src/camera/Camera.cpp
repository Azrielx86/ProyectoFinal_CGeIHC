//
// Created by edgar on 10/6/2023.
//

#include "Camera.h"
namespace Camera
{

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed, bool locked)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	this->locked = locked;

	update();
}

glm::vec3 Camera::getCameraPosition() { return this->position; }
glm::vec3 Camera::getCameraDirection() { return glm::normalize(front); }
glm::mat4 Camera::calculateViewMatrix() { return glm::lookAt(position, position + front, up); }

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::mouseControl(Input::MouseInput &mouse)
{
	if (locked) return;
	float xChange = mouse.getXChange() * turnSpeed;
	float yChange = mouse.getYChange() * turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	update();
}

void Camera::keyControl(Input::KeyboardInput &keyboard, GLfloat deltaTime)
{
	if (locked) return;
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
GLfloat Camera::getYaw() const
{
	return yaw;
}
GLfloat Camera::getPitch() const
{
	return pitch;
}
} // namespace Camera
