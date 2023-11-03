//
// Created by edgar on 10/5/2023.
//

#include "Model.h"
void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "No se pudo cargar el modelo: " << importer.GetErrorString() << "\n";
	}
	
	directory = path.substr(0, path.find_last_of('/'));
}
