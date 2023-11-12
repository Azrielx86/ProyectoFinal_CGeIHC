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
	std::cout << "[ " << typeid(BoneModel).name() << " ] Modelo cargado " << modelPath << "\n";
}
void BoneModel::loadNode(aiNode *node, const aiScene *scene)
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
							std::cerr << "Failed to load texture: " << file;
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
const std::unordered_map<std::string, BoneInfo> &BoneModel::getMBoneInfoMap() const
{
	return m_BoneInfoMap;
}

int BoneModel::getMBoneCount() const
{
	return m_BoneCount;
}
void BoneModel::setVertexBoneDataToDefault(Vertex &vtx)
{
	for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
	{
		vtx.m_BoneIDs[i] = -1;
		vtx.m_Weights[i] = 0.0f;
	}
}
} // namespace Model