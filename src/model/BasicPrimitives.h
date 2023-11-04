//
// Created by edgar on 10/1/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_BASICPRIMITIVES_H
#define PROYECTOFINAL_CGEIHC_BASICPRIMITIVES_H

#include "BasicMesh.h"
#include <unordered_map>
namespace Model
{

class BasicPrimitives
{
  public:
	enum Primitives
	{
		SINGLE_TRIANGLE,
		FLOOR
	};

	BasicPrimitives();
	void CreatePrimitives();
	BasicMesh *getPrimitive(BasicPrimitives::Primitives p);
	virtual ~BasicPrimitives();

  private:
	void CreateSingleTriangle();
	std::unordered_map<int, BasicMesh *> meshMap;
};
} // namespace Model
#endif // PROYECTOFINAL_CGEIHC_BASICPRIMITIVES_H
