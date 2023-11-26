//
// Created by edgar on 11/26/2023.
//

#include "PlayerCamera.h"

namespace Camera
{
PlayerCamera::PlayerCamera(Entity::Player *target)
{
	this->target = target;
	worldUp = {0, 1, 0};
	yaw = 0;
	pitch = 15;
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	distanceFromTarget = 20;
}

void PlayerCamera::MouseControl(Input::MouseInput &mouse)
{
	// Calcular el pitch
	float pitchChange = -mouse.getYChange() * 0.1f;
	pitch -= pitchChange;
	// Angulo alrededor
	float angleChange = mouse.getXChange() * 0.3f;
	angleAroundTarget -= angleChange;

	Update();
}

void PlayerCamera::KeyControl(Input::KeyboardInput &keyboard)
{
	yaw = 180 - target->getRotation().y + angleAroundTarget;
	
	// Calcular distancia
	float hdist = distanceFromTarget * std::cos(glm::radians(pitch));
	float vdist = distanceFromTarget * std::sin(glm::radians(pitch));

	// Calcular la posicion de la camara
	float theta = target->getRotation().y - angleAroundTarget;
	float offsetX = hdist * std::sin(glm::radians(theta));
	float offsetZ = hdist * std::cos(glm::radians(theta));
	position.x = target->getPosition().x - offsetX;
	position.y = target->getPosition().y + vdist + 2;
	position.z = target->getPosition().z - offsetZ;
}

void PlayerCamera::Update()
{
	//	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	//	front.y = sin(glm::radians(pitch));
	//	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	//	front = glm::normalize(front);
	//	right = glm::normalize(glm::cross(front, worldUp));
	//	up = glm::normalize(glm::cross(right, front));
}

glm::vec3 PlayerCamera::getCameraPosition() { return this->position; }

glm::vec3 PlayerCamera::getCameraDirection() { return glm::normalize(front); }

glm::mat4 PlayerCamera::calculateViewMatrix()
{
	glm::mat4 vm(1.0f);
	vm = glm::rotate(vm, (float) glm::radians(pitch), {1, 0, 0});
	vm = glm::rotate(vm, (float) glm::radians(yaw), {0, 1, 0});
	glm::vec3 negPos = {-position.x, -position.y, -position.z};
	vm = glm::translate(vm, negPos);
	return vm;
}

} // namespace Camera