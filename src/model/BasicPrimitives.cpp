//
// Created by edgar on 10/1/2023.
//

#include "BasicPrimitives.h"

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
	// clang-format on

	auto mesh = new BasicMesh();
	mesh->CreateMesh(vertices, idx, 24, 3);
	this->meshMap[Primitives::SINGLE_TRIANGLE] = mesh;
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
