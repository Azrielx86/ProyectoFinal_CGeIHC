//
// Created by edgar on 11/2/2023.
//

#include "CameraCollection.h"

namespace Camera
{
Camera *CameraCollection::getAcviveCamera() const { return activeCamera; }
CameraCollection &CameraCollection::addCamera(Camera *camera)
{
	cameras.push_back(camera);
	if (activeCamera == nullptr)
	{
		activeCamera = cameras.front();
		cameraIndex = 0;
	}
	return *this;
}
Camera *CameraCollection::switchCamera()
{
	cameraIndex++;
	if (cameraIndex >= (int) cameras.size())
		cameraIndex = 0;
	activeCamera = cameras.at(cameraIndex);
	std::cout << "[ " << typeid(CameraCollection).name() << " ] Camera changed " << activeCamera << '\n';
	std::cout << "\t==== Camera Position: ("
	          << activeCamera->getCameraPosition().x << ", " << activeCamera->getCameraPosition().y << ", " << activeCamera->getCameraPosition().z << ")\n"
	          << "\t==== Pitch: " << activeCamera->getPitch() << " | Yaw: " << activeCamera->getYaw() << "\n";
	return activeCamera;
}
CameraCollection::~CameraCollection()
{
	for (auto &cam : cameras)
		delete cam;
}
} // namespace Camera