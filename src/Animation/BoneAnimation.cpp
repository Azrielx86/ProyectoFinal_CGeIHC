//
// Created by edgar on 11/23/2023.
//

#include "BoneAnimation.h"

namespace Animation
{
BoneAnimation::BoneAnimation(const std::string &animationPath, Model::BoneModel *model)
{
	Assimp::Importer importer;
	auto scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
	assert(scene && scene->mRootNode);
	auto animation = scene->mAnimations[0];
	m_Duration = (float) animation->mDuration;
	m_TicksPerSecond = (int) animation->mTicksPerSecond;
	auto globalTransformation = scene->mRootNode->mTransformation;
	globalTransformation = globalTransformation.Inverse();
	ReadHeirarchyData(m_RootNode, scene->mRootNode);
	ReadMissingBones(animation, *model);
}

Model::Bone *BoneAnimation::FindBone(const std::string &name)
{
	auto iter = std::find_if(m_Bones.begin(), m_Bones.end(),
	                         [&](const Model::Bone &Bone)
	                         {
		                         return Bone.GetBoneName() == name;
	                         });
	if (iter == m_Bones.end())
		return nullptr;
	else
		return &(*iter);
}

void BoneAnimation::ReadMissingBones(const aiAnimation *animation, Model::BoneModel &model)
{
	int size = (int) animation->mNumChannels;
	auto &boneInfoMap = model.getMBoneInfoMap();
	auto &boneCount = model.getMBoneCounter();

	for (int i = 0; i < size; i++)
	{
		auto channel = animation->mChannels[i];
		std::string boneName = channel->mNodeName.data;

		if (boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			boneInfoMap[boneName].id = boneCount;
			boneCount++;
		}
		m_Bones.emplace_back(channel->mNodeName.data,
		                       boneInfoMap[channel->mNodeName.data].id, channel);
	}

	m_BoneInfoMap = boneInfoMap;
}

void BoneAnimation::ReadHeirarchyData(AssimpNodeData &dest, const aiNode *src) // NOLINT(*-no-recursion)
{
	assert(src);
	dest.name = src->mName.data;
	dest.transformation = Utils::Assimp2Glm::ConvertMatrix(src->mTransformation);
	dest.childrenCount = src->mNumChildren;

	for (int i = 0; i < src->mNumChildren; ++i)
	{
		AssimpNodeData newData;
		ReadHeirarchyData(newData, src->mChildren[i]);
		dest.children.push_back(newData);
	}
}
} // namespace Animation