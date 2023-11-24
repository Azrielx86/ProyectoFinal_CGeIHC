//
// Created by edgar on 11/23/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_BONEANIMATOR_H
#define PROYECTOFINAL_CGEIHC_BONEANIMATOR_H

#include "../model/Bone.h"
#include "../model/BoneModel.h"
#include "BoneAnimation.h"
#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace Animation
{

class BoneAnimator
{
  public:
	explicit BoneAnimator(BoneAnimation *animation);
	void UpdateAnimation(float dt);
	void PlayAnimation(BoneAnimation *pAnimation);
	void CalculateBoneTransform(const AssimpNodeData *node, glm::mat4 parentTransform);
	std::vector<glm::mat4> GetFinalBoneMatrices();

  private:
	std::vector<glm::mat4> m_FinalBoneMatrices;
	BoneAnimation *m_CurrentAnimation{nullptr};
	float m_CurrentTime{0.0f};
	float m_DeltaTime{0.0f};
};

} // namespace Animation

#endif // PROYECTOFINAL_CGEIHC_BONEANIMATOR_H
