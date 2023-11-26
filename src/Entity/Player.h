//
// Created by edgar on 11/25/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_PLAYER_H
#define PROYECTOFINAL_CGEIHC_PLAYER_H

#include "../GlobalConstants.h"
#include "../input/KeyboardInput.h"
#include "Entity.h"
#include <boost/format.hpp>
#include <glm/gtx/string_cast.hpp>

extern float deltaTime;

namespace Entity
{

class Player : public Entity
{
  public:
	Player(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale);

	void IncreasePosition(float dx, float dy, float dz) override;
	void IncreaseRotation(float dx, float dy, float dz) override;
	void Move();

  private:
	float currentSpeed = 0.0f;
	float currentTurnSpeed = 0.0f;
	bool moving = false;

  public:
	bool isMoving() const;

  private:
	void CheckInput(Input::KeyboardInput &keyboard);
};

} // namespace Entity

#endif // PROYECTOFINAL_CGEIHC_PLAYER_H
