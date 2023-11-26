//
// Created by edgar on 11/26/2023.
//

#include "StaticCamera.h"

namespace Camera
{
StaticCamera::StaticCamera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void StaticCamera::MouseControl([[maybe_unused]] Input::MouseInput &mouse)
{
}

void StaticCamera::KeyControl([[maybe_unused]] Input::KeyboardInput &keyboard)
{
}

void StaticCamera::Update()
{
}

glm::vec3 StaticCamera::getCameraPosition() { return this->position; }

glm::vec3 StaticCamera::getCameraDirection() { return glm::normalize(front); }

glm::mat4 StaticCamera::calculateViewMatrix() { return glm::lookAt(position, position + front, up); }

} // namespace Camera