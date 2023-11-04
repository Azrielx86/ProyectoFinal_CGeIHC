//
// Created by edgar on 11/3/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_MODELCOLLECTION_H
#define PROYECTOFINAL_CGEIHC_MODELCOLLECTION_H

#include "Model.h"
#include <omp.h>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <execution>

namespace Model
{

class ModelCollection
{
  public:
	explicit ModelCollection() = default;
	ModelCollection& addModel(int modelId, const std::string& modelPath);
	void loadModels();
	Model* getModel(int modelId);
  private:
	std::unordered_map<int, std::string> modelPaths;
	std::unordered_map<int, Model*> models;
};

} // namespace Model

#endif // PROYECTOFINAL_CGEIHC_MODELCOLLECTION_H
