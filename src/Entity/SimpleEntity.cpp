//
// Created by edgar on 11/11/2023.
//

#include "SimpleEntity.h"

namespace Entity
{
const glm::vec3 &SimpleEntity::getPosition() const { return position; }
const glm::vec3 &SimpleEntity::getRotation() const { return rotation; }

SimpleEntity &SimpleEntity::move(Entity::Axis axis, float mov)
{
	switch (axis)
	{
	case X:
		position.x += mov;
		break;
	case Y:
		position.y += mov;
		break;
	case Z:
		position.z += mov;
		break;
	}
	return *this;
}

SimpleEntity &SimpleEntity::rotate(Entity::Axis axis, float mov)
{
	switch (axis)
	{
	case X:
		rotation.x += mov;
		break;
	case Y:
		rotation.y += mov;
		break;
	case Z:
		rotation.z += mov;
		break;
	}
	return *this;
}
void SimpleEntity::update(const bool *keys, float dt)
{
	auto kmp = Input::KeyboardInput::GetInstance().getCurrentKeymap();
	if (kmp->at(GLFW_KEY_UP).pressed)
		position.x += MOV_OFFSET * dt;
	if (kmp->at(GLFW_KEY_DOWN).pressed)
		position.x -= MOV_OFFSET * dt;
	if (kmp->at(GLFW_KEY_LEFT).pressed)
		position.z -= MOV_OFFSET * dt;
	if (kmp->at(GLFW_KEY_RIGHT).pressed)
		position.z += MOV_OFFSET * dt;
	if (kmp->at(GLFW_KEY_O).pressed)
		position.y += MOV_OFFSET * dt;
	if (kmp->at(GLFW_KEY_K).pressed)
		position.y -= MOV_OFFSET * dt;
}
} // namespace Entity