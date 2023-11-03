//
// Created by edgar on 10/6/2023.
//

#include "Camera.h"
namespace Camera
{

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

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

void Camera::mouseControl(Input::MouseInput& mouseInput)
{
	float xChange = mouseInput.getXChange() * turnSpeed;
	float yChange = mouseInput.getYChange() * turnSpeed;

	yaw += xChange;
	pitch += yChange;

#ifdef DEBUG
	std::cout << "Camera updated! [Yaw: " << yaw << " | Pitch : " << pitch << "]\r";
#endif

	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	update();
}
} // namespace Camera
