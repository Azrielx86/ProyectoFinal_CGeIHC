//
// Created by edgar on 11/2/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_OLDCAMERACOLLECTION_H
#define PROYECTOFINAL_CGEIHC_OLDCAMERACOLLECTION_H

#include "Camera.h"
#include <vector>
#include <iostream>

namespace Camera
{
class OldCameraCollection
{
  public:
	explicit OldCameraCollection()
	{
		cameraIndex = 0;
		activeCamera = nullptr;
	};
	~OldCameraCollection();
	OldCameraCollection &addCamera(Camera *camera);
	Camera *switchCamera();
	[[nodiscard]] Camera *getAcviveCamera() const;

  private:
	std::vector<Camera *> cameras;
	Camera *activeCamera{};
	int cameraIndex;
};

} // namespace Camera

#endif // PROYECTOFINAL_CGEIHC_OLDCAMERACOLLECTION_H
