//
// Created by edgar on 11/2/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_CAMERACOLLECTION_H
#define PROYECTOFINAL_CGEIHC_CAMERACOLLECTION_H

#include "Camera.h"
#include <vector>

namespace Camera
{
class CameraCollection
{
  public:
	explicit CameraCollection() = default;
	CameraCollection& addCamera(const Camera& camera);
	[[nodiscard]] Camera *getAcviveCamera() const;
  private:
	std::vector<Camera> cameras;
	Camera* acviveCamera;
};

} // namespace camera

#endif // PROYECTOFINAL_CGEIHC_CAMERACOLLECTION_H
