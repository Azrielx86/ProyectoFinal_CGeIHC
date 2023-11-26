//
// Created by edgar on 11/26/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_FREECAMERA_H
#define PROYECTOFINAL_CGEIHC_FREECAMERA_H

#include "ICamera.h"
#include "../input/KeyboardInput.h"
#include "../input/MouseInput.h"

extern float deltaTime;

namespace Camera
{

class FreeCamera : public ICamera
{
  public:
	FreeCamera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
	void MouseControl(Input::MouseInput &mouse) override;
	void KeyControl(Input::KeyboardInput &keyboard) override;
	void Update() override;
	glm::vec3 getCameraPosition() override;
	glm::vec3 getCameraDirection() override;
	glm::mat4 calculateViewMatrix() override;
  private:
	float moveSpeed;
	float turnSpeed;
	void keyControl();
	void mouseControl();
};

} // namespace Camera

#endif // PROYECTOFINAL_CGEIHC_FREECAMERA_H
