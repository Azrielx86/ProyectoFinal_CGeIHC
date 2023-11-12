//
// Created by edgar on 11/10/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_BONEMODEL_H
#define PROYECTOFINAL_CGEIHC_BONEMODEL_H

#include "../Utils/ImageUtils.h"
#include "../Utils/PathUtils.h"
#include "BoneMesh.h"
#include "Texture.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/MathFunctions.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace Model
{

struct BoneInfo
{
	int id;
	glm::mat4 offset;
};

class BoneModel
{
  public:
	explicit BoneModel(const std::string &path);
	~BoneModel();
	void loadModel();
	void render();
	void clear();

  private:
	void loadNode(aiNode *node, const aiScene *scene);
	void loadMesh(aiMesh *mesh, [[maybe_unused]] const aiScene *scene);
	void loadMaterials(const aiScene *scene);
	void setVertexBoneDataToDefault(Vertex& vtx);

	std::vector<BoneMesh *> meshList;
	std::vector<uint> meshesToTexturize;
	std::vector<Texture *> textureList;
	std::string modelPath;

  public:
	[[nodiscard]] const std::unordered_map<std::string, BoneInfo> &getMBoneInfoMap() const;
	[[nodiscard]] int getMBoneCount() const;

  private:
	std::unordered_map<std::string, BoneInfo> m_BoneInfoMap;
	int m_BoneCount;
};

} // namespace Model

#endif // PROYECTOFINAL_CGEIHC_BONEMODEL_H
