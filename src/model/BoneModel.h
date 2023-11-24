//
// Created by edgar on 11/10/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_BONEMODEL_H
#define PROYECTOFINAL_CGEIHC_BONEMODEL_H

#include "../Utils/ImageUtils.h"
#include "../Utils/PathUtils.h"
#include <boost/format.hpp>
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
	~BoneModel()
	{
	}
	void loadModel();
	void render();
	void clear();

  private:
	void loadNode(aiNode *node, const aiScene *scene);
	void loadMesh(aiMesh *mesh, [[maybe_unused]] const aiScene *scene);
	void loadMaterials(const aiScene *scene);
	static void setVertexBoneDataToDefault(Vertex& vtx);
	
	static void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
	void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
	
	static inline glm::mat4 AssimpMat2GlmMat(const aiMatrix4x4& from)
	{
		glm::mat4 to;
		//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
		to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
		to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
		to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
		to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
		return to;
	}

	std::vector<BoneMesh *> meshList;
	std::vector<uint> meshesToTexturize;
	std::vector<Texture *> textureList;
	std::string modelPath;

  public:
	[[nodiscard]] const std::unordered_map<std::string, BoneInfo> &getMBoneInfoMap() const;
	[[nodiscard]] int getMBoneCounter() const;

  private:
	std::unordered_map<std::string, BoneInfo> m_BoneInfoMap;
	int m_BoneCounter{};
};

} // namespace Model

#endif // PROYECTOFINAL_CGEIHC_BONEMODEL_H
