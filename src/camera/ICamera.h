//
// Created by edgar on 11/26/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_ICAMERA_H
#define PROYECTOFINAL_CGEIHC_ICAMERA_H

#include "../input/KeyboardInput.h"
#include "../input/MouseInput.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Camera
{

class ICamera
{
  protected:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	float pitch;
	float yaw;
	float roll;

  public:
	virtual void MouseControl(Input::MouseInput &mouse) = 0;
	virtual void KeyControl(Input::KeyboardInput &keyboard) = 0;
	virtual void Update() = 0;
	virtual void FixedUpdate() = 0;
	virtual glm::vec3 getCameraPosition() = 0;
	virtual glm::vec3 getCameraDirection() = 0;
	virtual glm::mat4 calculateViewMatrix() = 0;

	[[nodiscard]] float getYaw() const { return yaw; };

	[[nodiscard]] float getPitch() const { return pitch; };

	[[nodiscard]] glm::vec3 getUp() const { return up; };
};

} // namespace Camera

#endif // PROYECTOFINAL_CGEIHC_ICAMERA_H
