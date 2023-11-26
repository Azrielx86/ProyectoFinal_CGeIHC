//
// Created by edgar on 11/26/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_PLAYERCAMERA_H
#define PROYECTOFINAL_CGEIHC_PLAYERCAMERA_H

#include "../Entity/Player.h"
#include "ICamera.h"

namespace Camera
{

class PlayerCamera: public ICamera
{
  public:
	PlayerCamera(Entity::Player* target);
	void MouseControl(Input::MouseInput &mouse) override;
	void KeyControl(Input::KeyboardInput &keyboard) override;
	void Update() override;
	glm::vec3 getCameraPosition() override;
	glm::vec3 getCameraDirection() override;
	glm::mat4 calculateViewMatrix() override;
  private:
	float distanceFromTarget = 50;
	float angleAroundTarget = 0;
	// El pitch es el ángulo vertical para el objetivo
	Entity::Player* target;
};

} // namespace Camera

#endif // PROYECTOFINAL_CGEIHC_PLAYERCAMERA_H
