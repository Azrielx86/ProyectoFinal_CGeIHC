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
/**
 * Manages a multiple model collection.
 */
class ModelCollection
{
  public:
	explicit ModelCollection() = default;
	~ModelCollection();
	
	/**
	 * Insert a model into the collection.
	 * @param modelId An unique identifier for the model.
	 * @param modelPath File path from the model
	 * @return Current instance.
	 */
	ModelCollection& addModel(int modelId, const std::string& modelPath);
	
	/**
	 * Loads all the models from the collection.
	 * @warning Parallel loading have issues.
	 */
	void loadModels();
	Model& operator[](int index);
	
	/**
	 * @deprecated Use operator[] instead.
	 * @param modelId 
	 * @return 
	 */
	Model* getModel(int modelId);
  private:
	std::unordered_map<int, std::string> modelPaths;
	std::unordered_map<int, Model*> models;
};

} // namespace Model

#endif // PROYECTOFINAL_CGEIHC_MODELCOLLECTION_H
