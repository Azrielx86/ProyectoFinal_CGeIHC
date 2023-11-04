//
// Created by edgar on 10/5/2023.
//

#include "Model.h"

namespace Model
{
Model::Model(const std::string &path)
{
	modelPath = path;
	if (!fs::exists(path))
		std::cerr << "[Model - Warning] El modelo "
		          << path << " no existe. \n";
}

void Model::loadModel()
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
	std::cout << "Modelo cargado: " << modelPath << "\n";
}

void Model::loadNode(aiNode *node, const aiScene *scene)
{
	for (uint i = 0; i < node->mNumMeshes; i++)
		loadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	for (uint i = 0; i < node->mNumChildren; i++)
		loadNode(node->mChildren[i], scene);
}

void Model::loadMesh(aiMesh *mesh, [[maybe_unused]] const aiScene *scene)
{
	std::vector<GLfloat> vertices;
	std::vector<uint> indices;
	for (uint i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.insert(vertices.end(), {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z});
		if (mesh->mTextureCoords[0])
			vertices.insert(vertices.end(), {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y});
		else
			vertices.insert(vertices.end(), {0.0f, 0.0f});
		vertices.insert(vertices.end(), {-mesh->mVertices[i].x, -mesh->mVertices[i].y, -mesh->mVertices[i].z});
	}

	for (uint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (uint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	auto nMesh = new Mesh();
	nMesh->createMesh(vertices.data(), indices.data(), vertices.size(), indices.size());
	meshList.push_back(nMesh);
	meshesToTexturize.push_back(mesh->mMaterialIndex);
}
void Model::loadMaterials(const aiScene *scene)
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
				auto file = fs::path(path.data).filename().string();
				auto searchPath = Utils::PathUtils::getTexturesPath() + "/" + file;
				textureList[i] = new Texture(searchPath.c_str());
				if (fs::exists(searchPath))
				{
					if (!textureList[i]->LoadTexture(true))
					{
						std::cerr << "Failed to load texture: " << file;
						delete textureList[i];
						textureList[i] = nullptr;
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
void Model::render()
{
	for (uint i = 0; i < meshList.size(); i++)
	{
		uint matIdx = meshesToTexturize[i];
		if (matIdx < textureList.size() && textureList[matIdx])
			textureList[matIdx]->UseTexture();
		meshList[i]->render();
	}
}
void Model::clear()
{
	for (auto &mesh : meshList)
	{
		if (!mesh) continue;
		delete mesh;
		mesh = nullptr;
	}

	for (auto &texture : textureList)
	{
		if (!texture) continue;
		delete texture;
		texture = nullptr;
	}
}
Model::~Model()
{
	clear();
}
} // namespace Model
