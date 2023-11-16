//
// Created by edgar on 11/2/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_CAMERACOLLECTION_H
#define PROYECTOFINAL_CGEIHC_CAMERACOLLECTION_H

#include "Camera.h"
#include <iostream>
#include <vector>
#include <iostream>

namespace Camera
{
class CameraCollection
{
  public:
	explicit CameraCollection()
	{
		cameraIndex = 0;
		activeCamera = nullptr;
	};
	~CameraCollection();
	CameraCollection &addCamera(Camera *camera);
	Camera *switchCamera();
	[[nodiscard]] Camera *getAcviveCamera() const;

  private:
	std:: vector<Camera *> cameras;
	Camera *activeCamera{};
	int cameraIndex;
};

} // namespace Camera

#endif // PROYECTOFINAL_CGEIHC_CAMERACOLLECTION_H
