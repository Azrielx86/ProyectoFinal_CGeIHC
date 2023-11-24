//
// Created by edgar on 11/23/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_BONE_H
#define PROYECTOFINAL_CGEIHC_BONE_H

#include "../Utils/Assimp2Glm.h"
#include <assimp/anim.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <string>
#include <vector>

namespace Model
{

struct KeyPosition
{
	glm::vec3 position;
	float timeStamp;
};

struct KeyRotation
{
	glm::quat orientation;
	float timeStamp;
};

struct KeyScale
{
	glm::vec3 scale;
	float timeStamp;
};

class Bone
{
  private:
	std::vector<KeyPosition> m_Positions;
	std::vector<KeyRotation> m_Rotations;
	std::vector<KeyScale> m_Scales;
	int m_NumPositions;
	int m_NumRotations;
	int m_NumScalings;

	glm::mat4 m_LocalTransform = glm::mat4 (1.0f);
	std::string m_Name;
	int m_ID;

  public:
	Bone(std::string name, int Id, const aiNodeAnim *channel);
	void Update(float time);
	int GetPositionIndex(float time);
	int GetRotationIndex(float time);
	int GetScaleIndex(float time);
	[[nodiscard]] glm::mat4 GetLocalTransform() { return m_LocalTransform; }
	[[nodiscard]] std::string GetBoneName() const { return m_Name; }
	[[nodiscard]] int GetBoneID() const { return m_ID; }

  private:
	static float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float time);
	glm::mat4 InterpolatePosition(float time);
	glm::mat4 InterpolateRotation(float time);
	glm::mat4 InterpolateScaling(float time);
};

} // namespace Model

#endif // PROYECTOFINAL_CGEIHC_BONE_H
