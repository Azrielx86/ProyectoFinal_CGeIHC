//
// Created by edgar on 11/10/2023.
//

#include "BoneModel.h"

namespace Model
{
void BoneModel::render()
{
	for (uint i = 0; i < meshList.size(); i++)
	{
		uint matIdx = meshesToTexturize[i];
		if (matIdx < textureList.size() && textureList[matIdx])
			textureList[matIdx]->UseTexture();
		meshList[i]->render();
	}
}
BoneModel::BoneModel(const std::string &path)
{
	modelPath = path;
	if (!std::filesystem::exists(path))
		std::cerr << "[ " << typeid(BoneModel).name() << " ] El modelo "
		          << path << " no existe. \n";
	m_BoneCounter = 0;
}
void BoneModel::loadModel()
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "No se pudo cargar el modelo: " << importer.GetErrorString() << "\n";
		return;
	}

	loadNode(scene->mRootNode, scene);
	loadMaterials(scene);
	std::cout << boost::format("\x1B[32m[ %s ] Modelo (Bone Rig) cargado (%s) con %d huesos.\x1B[0m\n") % typeid(BoneModel).name() % modelPath % m_BoneCounter;
}

void BoneModel::loadNode(aiNode *node, const aiScene *scene) // NOLINT(*-no-recursion)
{
	for (uint i = 0; i < node->mNumMeshes; i++)
		loadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	for (uint i = 0; i < node->mNumChildren; i++)
		loadNode(node->mChildren[i], scene);
}
void BoneModel::loadMesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<uint> indices;
	std::vector<Vertex> vertices;

	for (uint i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex{};
		vertex.Position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
		vertex.Normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
		setVertexBoneDataToDefault(vertex);
		if (mesh->mTextureCoords[0])
		{
			vertex.TexCoords = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
			//			vertex.Tangent = {mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z};
			//			vertex.Bitangent = {mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z};
		}
		else
			vertex.TexCoords = {0.0f, 0.0f};
		vertices.push_back(vertex);
	}

	for (uint i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (uint j = 0; j < face.mNumIndices; ++j)
			indices.push_back(face.mIndices[j]);
	}

	ExtractBoneWeightForVertices(vertices, mesh, scene);
	meshList.push_back(new BoneMesh(vertices, indices));
	meshesToTexturize.push_back(mesh->mMaterialIndex);
}
void BoneModel::loadMaterials(const aiScene *scene)
{
	textureList.resize(scene->mNumMaterials);
	for (uint i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial *material = scene->mMaterials[i];
		textureList[i] = nullptr;
		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				auto file = std::filesystem::path(path.data).filename().string();
				auto searchPath = Utils::PathUtils::getTexturesPath() + "/" + file;
				textureList[i] = new Texture(searchPath.c_str());
				if (std::filesystem::exists(searchPath))
				{
					auto type = Utils::ImageUtils::GetPngChannel(searchPath);
					if (type != Utils::ImageUtils::CHANNEL_TYPE::ERROR)
					{
						if (!textureList[i]->LoadTexture(type == Utils::ImageUtils::CHANNEL_TYPE::RGBA))
						{
							std::cerr << boost::format("[ %s ] Failed to load texture: %s") % typeid(BoneModel).name() % file;
							delete textureList[i];
							textureList[i] = nullptr;
						}
					}
				}
			}
		}

		if (!textureList[i])
		{
			auto texturePath = Utils::PathUtils::getTexturesPath() + "/plain.png";
			textureList[i] = new Texture(texturePath.c_str());
			textureList[i]->LoadTexture(true);
		}
	}
}
std::unordered_map<std::string, BoneInfo> &BoneModel::getMBoneInfoMap()
{
	return m_BoneInfoMap;
}

int &BoneModel::getMBoneCounter()
{
	return m_BoneCounter;
}

void BoneModel::setVertexBoneDataToDefault(Vertex &vtx)
{
	for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
	{
		vtx.m_BoneIDs[i] = -1;
		vtx.m_Weights[i] = 0.0f;
	}
}
void BoneModel::SetVertexBoneData(Vertex &vertex, int boneID, float weight)
{
	for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
	{
		if (vertex.m_BoneIDs[i] < 0)
		{
			vertex.m_Weights[i] = weight;
			vertex.m_BoneIDs[i] = boneID;
			break;
		}
	}
}
void BoneModel::ExtractBoneWeightForVertices(std::vector<Vertex> &vertices, aiMesh *mesh, const aiScene *scene)
{
	for (uint boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
	{
		int boneID = -1;
		std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
		if (m_BoneInfoMap.find(boneName) == m_BoneInfoMap.end())
		{
			BoneInfo newBoneInfo{};
			newBoneInfo.id = m_BoneCounter;
			newBoneInfo.offset = AssimpMat2GlmMat(mesh->mBones[boneIndex]->mOffsetMatrix);
			m_BoneInfoMap[boneName] = newBoneInfo;
			boneID = m_BoneCounter;
			m_BoneCounter++;
			std::cout << boost::format("[ %s ] Bone found: %s (%d)\n") % typeid(BoneModel).name() % boneName % m_BoneCounter;
		}
		else
		{
			boneID = m_BoneInfoMap[boneName].id;
		}
		assert(boneID != -1);
		auto weights = mesh->mBones[boneIndex]->mWeights;
		auto numWeights = mesh->mBones[boneIndex]->mNumWeights;

		for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
		{
			auto vertexId = weights[weightIndex].mVertexId;
			auto weight = weights[weightIndex].mWeight;
			assert(vertexId <= vertices.size());
			SetVertexBoneData(vertices[vertexId], boneID, weight);
		}
	}
}
} // namespace Model