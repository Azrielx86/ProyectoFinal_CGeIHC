//
// Created by edgar on 11/25/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_ENTITY_H
#define PROYECTOFINAL_CGEIHC_ENTITY_H

#include <glm/glm.hpp>

extern float deltaTime;

namespace Entity
{

class Entity
{
  private:
	glm::vec3 position{};
	glm::vec3 rotation{};
	glm::vec3 scale{};

  public:
	Entity(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale);
	const glm::vec3 &getPosition() const;
	void setPosition(const glm::vec3 &pos);
	const glm::vec3 &getRotation() const;
	void setRotation(const glm::vec3 &rot);
	const glm::vec3 &getScale() const;
	void setScale(const glm::vec3 &sca);
	virtual void IncreasePosition(float dx, float dy, float dz);
	virtual void IncreaseRotation(float dx, float dy, float dz);
};

} // namespace Entity

#endif // PROYECTOFINAL_CGEIHC_ENTITY_H
