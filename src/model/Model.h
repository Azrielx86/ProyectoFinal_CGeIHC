//
// Created by edgar on 10/5/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_MODEL_H
#define PROYECTOFINAL_CGEIHC_MODEL_H

#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

class Model
{
  public:
	Model(char *path) {}

  private:
	std::vector<Mesh> Meshes;
	std::string directory;
	void loadModel(std::string path);
};

#endif // PROYECTOFINAL_CGEIHC_MODEL_H
