//
// Created by edgar on 10/1/2023.
//

#include "BasicPrimitives.h"
namespace Model
{
BasicPrimitives::BasicPrimitives() {}

void BasicPrimitives::CreatePrimitives()
{
	CreateSingleTriangle();
}

void BasicPrimitives::CreateSingleTriangle()
{
	// clang-format off
    float vertices[] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    };

	unsigned int idx[] = {
		1, 2, 3
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};
	
	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	// clang-format on

	auto mesh = new BasicMesh();
	mesh->CreateMesh(vertices, idx, 24, 3);
	meshMap[Primitives::SINGLE_TRIANGLE] = mesh;

	auto floor = new BasicMesh();
	floor->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshMap[Primitives::FLOOR] = floor;
}
BasicMesh *BasicPrimitives::getPrimitive(BasicPrimitives::Primitives p)
{
	return this->meshMap[p];
}

BasicPrimitives::~BasicPrimitives()
{
	for (const auto &item : meshMap)
		delete item.second;
}
} // namespace Model
