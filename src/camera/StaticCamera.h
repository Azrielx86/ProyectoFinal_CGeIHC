//
// Created by edgar on 11/26/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_STATICCAMERA_H
#define PROYECTOFINAL_CGEIHC_STATICCAMERA_H

#include "ICamera.h"

namespace Camera
{

class StaticCamera : public ICamera
{
  public:
	StaticCamera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch);
	void MouseControl(Input::MouseInput &mouse) override;
	void KeyControl(Input::KeyboardInput &keyboard) override;
	void Update() override;
	glm::vec3 getCameraPosition() override;
	glm::vec3 getCameraDirection() override;
	glm::mat4 calculateViewMatrix() override;
};

} // namespace Camera

#endif // PROYECTOFINAL_CGEIHC_STATICCAMERA_H
