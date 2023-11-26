//
// Created by edgar on 11/25/2023.
//

#include "Entity.h"

namespace Entity
{
const glm::vec3 &Entity::getPosition() const
{
	return position;
}
void Entity::setPosition(const glm::vec3 &pos)
{
	Entity::position = pos;
}
const glm::vec3 &Entity::getRotation() const
{
	return rotation;
}
void Entity::setRotation(const glm::vec3 &rot)
{
	Entity::rotation = rot;
}
const glm::vec3 &Entity::getScale() const
{
	return scale;
}
void Entity::setScale(const glm::vec3 &sca)
{
	Entity::scale = sca;
}
Entity::Entity(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale) : position(position), rotation(rotation), scale(scale) {}

void Entity::IncreasePosition(float dx, float dy, float dz)
{
	this->position.x += dx;
	this->position.y += dy;
	this->position.z += dz;
}

void Entity::IncreaseRotation(float dx, float dy, float dz)
{
	this->rotation.x += dx;
	this->rotation.y += dy;
	this->rotation.z += dz;
}
} // namespace Entity