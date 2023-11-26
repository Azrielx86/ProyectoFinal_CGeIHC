//
// Created by edgar on 11/25/2023.
//

#include "Player.h"

namespace Entity
{
Player::Player(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale) : Entity(position, rotation, scale) {}

void Player::IncreasePosition(float dx, float dy, float dz)
{
	Entity::IncreasePosition(dx, dy, dz);
}

void Player::IncreaseRotation(float dx, float dy, float dz)
{
	Entity::IncreaseRotation(dx, dy, dz);
}

void Player::CheckInput(Input::KeyboardInput &keyboard)
{
	if (keyboard.getCurrentKeymap()->at(GLFW_KEY_W).pressed)
		this->currentSpeed = MOVE_SPEED;
	else if (keyboard.getCurrentKeymap()->at(GLFW_KEY_S).pressed)
		this->currentSpeed = -MOVE_SPEED;
	else
		this->currentSpeed = 0.0f;

	if (keyboard.getCurrentKeymap()->at(GLFW_KEY_D).pressed)
		this->currentTurnSpeed = -TURN_SPEED;
	else if (keyboard.getCurrentKeymap()->at(GLFW_KEY_A).pressed)
		this->currentTurnSpeed = TURN_SPEED;
	else
		this->currentTurnSpeed = 0.0f;
}

void Player::Move()
{
	CheckInput(Input::KeyboardInput::GetInstance());
	Player::IncreaseRotation(0, currentTurnSpeed * deltaTime, 0);
	float distance = currentSpeed * deltaTime;
	float dx = distance * std::sin(glm::radians(this->getRotation().y));
	float dz = distance * std::cos(glm::radians(this->getRotation().y));
	Player::IncreasePosition(dx, 0, dz);
	std::cout << boost::format("Player position: %s\r") % glm::to_string(this->getPosition());
}

bool Player::isMoving() const { return currentSpeed != 0.0f; }
} // namespace Entity