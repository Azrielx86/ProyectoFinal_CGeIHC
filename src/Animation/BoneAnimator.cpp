//
// Created by edgar on 11/23/2023.
//

#include "BoneAnimator.h"

namespace Animation
{
BoneAnimator::BoneAnimator(BoneAnimation *animation)
{
	m_CurrentTime = 0;
	m_CurrentAnimation = animation;
	m_FinalBoneMatrices.reserve(100);
	for (int i = 0; i < 100; ++i)
		m_FinalBoneMatrices.emplace_back(1.0f);
}

void BoneAnimator::UpdateAnimation(float dt)
{
	m_DeltaTime = dt;
	if (m_CurrentAnimation)
	{
		m_CurrentTime += (float) m_CurrentAnimation->GetTicksPerSecond() * dt;
		m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
		CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
	}
}

void BoneAnimator::PlayAnimation(BoneAnimation *pAnimation)
{
	if (m_CurrentAnimation == pAnimation) return;
	m_CurrentAnimation = pAnimation;
	m_CurrentTime = 0.0f;
}

void BoneAnimator::CalculateBoneTransform(const AssimpNodeData *node, glm::mat4 parentTransform)
{
	auto nodeName = node->name;
	auto nodeTransform = node->transformation;
	auto bone = m_CurrentAnimation->FindBone(nodeName);
	if (bone)
	{
		bone->Update(m_CurrentTime);
		nodeTransform = bone->GetLocalTransform();
	}

	auto globalTransformation = parentTransform * nodeTransform;

	auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
	if (boneInfoMap.find(nodeName) != boneInfoMap.end())
	{
		int index = boneInfoMap[nodeName].id;
		glm::mat4 offset = boneInfoMap[nodeName].offset;
		m_FinalBoneMatrices[index] = globalTransformation * offset;
	}

	for (int i = 0; i < node->childrenCount; i++)
		CalculateBoneTransform(&node->children[i], globalTransformation);
}

std::vector<glm::mat4> BoneAnimator::GetFinalBoneMatrices() { return m_FinalBoneMatrices; }

} // namespace Animation