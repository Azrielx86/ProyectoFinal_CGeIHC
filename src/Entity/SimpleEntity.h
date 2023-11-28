//
// Created by edgar on 11/11/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_SIMPLEENTITY_H
#define PROYECTOFINAL_CGEIHC_SIMPLEENTITY_H

#include <functional>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <GLFW/glfw3.h>
#include "../input/KeyboardInput.h"
#define MOV_OFFSET 0.3f

namespace Entity
{
class SimpleEntity
{
	enum Axis
	{
		X,
		Y,
		Z
	};

  public:
	[[nodiscard]] const glm::vec3 &getPosition() const;
	[[nodiscard]] const glm::vec3 &getRotation() const;
	SimpleEntity &move(Axis axis, float mov);
	SimpleEntity &rotate(Axis axis, float mov);
	//	SimpleEntity &moveX(float mov);
	//	SimpleEntity &moveY(float mov);
	//	SimpleEntity &moveZ(float mov);
	//	SimpleEntity &rotateX(float degrees);
	//	SimpleEntity &rotateY(float degrees);
	//	SimpleEntity &rotateZ(float degrees);
	void update(const bool *keys, float dt);

  private:
	glm::vec3 position = {0.0f, 0.0f, 0.0f};
	glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
	glm::vec3 direction = {0.0f, 0.0f, 0.0f};
};

} // namespace Entity

#endif // PROYECTOFINAL_CGEIHC_SIMPLEENTITY_H
