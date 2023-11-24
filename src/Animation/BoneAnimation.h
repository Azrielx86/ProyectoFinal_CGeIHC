//
// Created by edgar on 11/23/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_BONEANIMATION_H
#define PROYECTOFINAL_CGEIHC_BONEANIMATION_H

#include "../model/Bone.h"
#include "../model/BoneModel.h"
#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace Animation
{

struct AssimpNodeData
{
	glm::mat4 transformation;
	std::string name;
	int childrenCount;
	std::vector<AssimpNodeData> children;
};

class BoneAnimation
{
  public:
	BoneAnimation() = default;
	BoneAnimation(const std::string &animationPath, Model::BoneModel *model);
	Model::Bone *FindBone(const std::string &name);

	[[nodiscard]] int GetTicksPerSecond() const { return m_TicksPerSecond; }
	[[nodiscard]] float GetDuration() const { return m_Duration; }
	const AssimpNodeData &GetRootNode() { return m_RootNode; }
	const std::unordered_map<std::string, Model::BoneInfo> &GetBoneIDMap() { return m_BoneInfoMap; }

  private:
	void ReadMissingBones(const aiAnimation* animation, Model::BoneModel& model);
	void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);
	
	float m_Duration;
	int m_TicksPerSecond;
	std::vector<Model::Bone> m_Bones;
	AssimpNodeData m_RootNode;
	std::unordered_map<std::string, Model::BoneInfo> m_BoneInfoMap;
};

} // namespace Animation

#endif // PROYECTOFINAL_CGEIHC_BONEANIMATION_H
