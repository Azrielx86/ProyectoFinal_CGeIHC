//
// Created by edgar on 11/2/2023.
//

#include "OldCameraCollection.h"

namespace Camera
{
Camera *OldCameraCollection::getAcviveCamera() const { return activeCamera; }

OldCameraCollection &OldCameraCollection::addCamera(Camera *camera)
{
	cameras.push_back(camera);
	if (activeCamera == nullptr)
	{
		activeCamera = cameras.front();
		cameraIndex = 0;
	}
	return *this;
}
Camera *OldCameraCollection::switchCamera()
{
	cameraIndex++;
	if (cameraIndex >= (int) cameras.size())
		cameraIndex = 0;
	activeCamera = cameras.at(cameraIndex);
	std::cout << "[ " << typeid(OldCameraCollection).name() << " ] Camera changed " << activeCamera << '\n';
	std::cout << "\t==== Camera Position: ("
	          << activeCamera->getCameraPosition().x << ", " << activeCamera->getCameraPosition().y << ", " << activeCamera->getCameraPosition().z << ")\n"
	          << "\t==== Pitch: " << activeCamera->getPitch() << " | Yaw: " << activeCamera->getYaw() << "\n";
	return activeCamera;
}

OldCameraCollection::~OldCameraCollection()
{
	for (auto &cam : cameras)
		delete cam;
}
} // namespace Camera