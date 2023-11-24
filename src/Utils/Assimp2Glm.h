//
// Created by edgar on 11/23/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_ASSIMP2GLM_H
#define PROYECTOFINAL_CGEIHC_ASSIMP2GLM_H

#include <assimp/matrix4x4.h>
#include <assimp/quaternion.h>
#include <assimp/vector3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Utils
{

class Assimp2Glm
{
  public:
	static inline glm::mat4 ConvertMatrix(const aiMatrix4x4 &from)
	{
		// clang-format off
		glm::mat4 to;
		//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
		to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
		to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
		to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
		to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
		return to;
		// clang-format on
	}

	static inline glm::vec3 ConvertVector3(const aiVector3D &vec)
	{
		return {vec.x, vec.y, vec.z};
	}

	static inline glm::quat ConvertQuaternion(const aiQuaternion &pOrientation)
	{
		return {pOrientation.w, pOrientation.x, pOrientation.y, pOrientation.z};
	}
};

} // namespace Utils

#endif // PROYECTOFINAL_CGEIHC_ASSIMP2GLM_H
