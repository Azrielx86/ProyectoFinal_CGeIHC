//
// Created by edgar on 11/3/2023.
//

#include "ModelCollection.h"

namespace Model
{
ModelCollection &ModelCollection::addModel(int modelId, const std::string &modelPath)
{
	auto model = new Model(modelPath);
	models[modelId] = model;
	return *this;
}

void ModelCollection::loadModels()
{
#ifdef PARALLEL_LOADING
#warning "Parallel loading have issues."
	std::for_each(std::execution::par,
	              std::begin(models),
	              std::end(models),
	              [](std::pair<int, Model *> pair)
	              {
		              pair.second->loadModel();
	              });
#else
	for (auto &model : models)
		model.second->loadModel();
#endif
}

Model *ModelCollection::getModel(int modelId)
{
	if (models.find(modelId) == models.end())
		return nullptr;
	return models.at(modelId);
}

Model &ModelCollection::operator[](int index)
{
	return *models.at(index);
}
} // namespace Model