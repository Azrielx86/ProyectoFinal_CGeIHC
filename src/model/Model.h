//
// Created by edgar on 10/5/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_MODEL_H
#define PROYECTOFINAL_CGEIHC_MODEL_H

#include "Mesh.h"
#include "Texture.h"
#include "../Utils/PathUtils.h"
#include "../Utils/ImageUtils.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace Model
{
class Model
{
  public:
	explicit Model(const std::string &path);
	~Model();
	void loadModel();
	void render();
	void clear();

  private:
	void loadNode(aiNode *node, const aiScene *scene);
	void loadMesh(aiMesh *mesh, [[maybe_unused]] const aiScene *scene);
	void loadMaterials(const aiScene *scene);

	std::vector<Mesh *> meshList;
	std::vector<uint> meshesToTexturize;
	std::vector<Texture *> textureList;
	std::string modelPath;
};

} // namespace Model
#endif // PROYECTOFINAL_CGEIHC_MODEL_H
