//
// Created by edgar on 11/23/2023.
//

#include "Bone.h"

#include <utility>

namespace Model
{
Bone::Bone(std::string name, int Id, const aiNodeAnim *channel)
    : m_LocalTransform(1.0f), m_Name(std::move(name)), m_ID(Id)
{
	m_NumPositions = (int) channel->mNumPositionKeys;
	for (int i = 0; i < m_NumPositions; ++i)
	{
		auto aiPosition = channel->mPositionKeys[i].mValue;
		auto timeStamp = (float) channel->mPositionKeys[i].mTime;
		KeyPosition data{};
		data.position = Utils::Assimp2Glm::ConvertVector3(aiPosition);
		data.timeStamp = timeStamp;
		m_Positions.push_back(data);
	}

	m_NumRotations = (int) channel->mNumRotationKeys;
	for (int i = 0; i < m_NumRotations; ++i)
	{
		auto quat = channel->mRotationKeys[i].mValue;
		auto timeStamp = (float) channel->mRotationKeys[i].mTime;
		KeyRotation data{};
		data.orientation = Utils::Assimp2Glm::ConvertQuaternion(quat);
		data.timeStamp = timeStamp;
		m_Rotations.push_back(data);
	}

	m_NumScalings = (int) channel->mNumScalingKeys;
	for (int i = 0; i < m_NumScalings; ++i)
	{
		auto scale = channel->mScalingKeys[i].mValue;
		auto timeStamp = (float) channel->mScalingKeys[i].mTime;
		KeyScale data{};
		data.scale = Utils::Assimp2Glm::ConvertVector3(scale);
		data.timeStamp = timeStamp;
		m_Scales.push_back(data);
	}
}

glm::mat4 Bone::InterpolatePosition(float time)
{
	if (m_NumPositions == 1)
		return glm::translate(glm::mat4(1.0f), m_Positions[0].position);

	auto p0Index = GetPositionIndex(time);
	auto p1Index = p0Index + 1;
	auto scaleFactor = GetScaleFactor(m_Positions[p0Index].timeStamp, m_Positions[p1Index].timeStamp, time);
	auto finalPosition = glm::mix(m_Positions[p0Index].position, m_Positions[p1Index].position, scaleFactor);
	return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 Bone::InterpolateRotation(float time)
{
	if (m_NumRotations == 1)
	{
		auto rot = glm::normalize(m_Rotations[0].orientation);
		return glm::toMat4(rot);
	}

	auto p0Index = GetRotationIndex(time);
	auto p1Index = p0Index + 1;
	auto scaleFactor = GetScaleFactor(m_Rotations[p0Index].timeStamp, m_Rotations[p1Index].timeStamp, time);
	glm::quat finalRotation = glm::slerp(m_Rotations[p0Index].orientation, m_Rotations[p1Index].orientation, scaleFactor);
	finalRotation = glm::normalize(finalRotation);
	return glm::toMat4(finalRotation);
}

glm::mat4 Bone::InterpolateScaling(float time)
{
	if (m_NumScalings == 1)
		return glm::scale(glm::mat4(1.0f), m_Scales[0].scale);

	auto p0Index = GetScaleIndex(time);
	auto p1Index = p0Index + 1;
	auto scaleFactor = GetScaleFactor(m_Scales[p0Index].timeStamp, m_Scales[p1Index].timeStamp, time);
	auto finalScale = glm::mix(m_Scales[p0Index].scale, m_Scales[p1Index].scale, scaleFactor);
	return glm::scale(glm::mat4(1.0f), finalScale);
}

int Bone::GetPositionIndex(float time)
{
	for (int index = 0; index < m_NumPositions - 1; ++index)
	{
		if (time < m_Positions[index + 1].timeStamp)
			return index;
	}
	assert(0);
}

int Bone::GetRotationIndex(float time)
{
	for (int index = 0; index < m_NumRotations - 1; ++index)
	{
		if (time < m_Rotations[index + 1].timeStamp)
			return index;
	}
	assert(0);
}

int Bone::GetScaleIndex(float time)
{
	for (int index = 0; index < m_NumScalings - 1; ++index)
	{
		if (time < m_Scales[index + 1].timeStamp)
			return index;
	}
	assert(0);
}

float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float time)
{
	float scaleFactor = 0.0f;
	float midWayLength = time - lastTimeStamp;
	float framesDiff = nextTimeStamp - lastTimeStamp;
	scaleFactor = midWayLength / framesDiff;
	return scaleFactor;
}

void Bone::Update(float time)
{
	auto traslation = InterpolatePosition(time);
	auto rotation = InterpolateRotation(time);
	auto scale = InterpolateScaling(time);
	m_LocalTransform = traslation * rotation * scale;
}

} // namespace Model