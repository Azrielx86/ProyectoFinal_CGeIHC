//
// Created by edgar on 11/26/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_CAMERACOLLECTION_H
#define PROYECTOFINAL_CGEIHC_CAMERACOLLECTION_H
#include "ICamera.h"
#include <iostream>
#include <type_traits>
#include <vector>

namespace Camera
{
template <typename T>
class CameraCollection
{
	static_assert(std::is_base_of<ICamera, T>::value, "Collection must be ICamera-like type.");

  public:
	explicit CameraCollection()
	{
		cameraIndex = 0;
		activeCamera = nullptr;
	};

	~CameraCollection()
	{
//		for (auto &cam : cameras)
//			delete cam;
	};

	CameraCollection &addCamera(T *camera)
	{
		cameras.push_back(camera);
		if (activeCamera == nullptr)
		{
			activeCamera = cameras.front();
			cameraIndex = 0;
		}
		return *this;
	}

	T *switchCamera()
	{
		cameraIndex++;
		if (cameraIndex >= (int) cameras.size())
			cameraIndex = 0;
		activeCamera = cameras.at(cameraIndex);
		std::cout << "[ " << typeid(CameraCollection).name() << " ] Camera changed\n"
		          << "\t==== Type: " << typeid(*activeCamera).name() << "\n"
		          << "\t==== Position: ("
		          << activeCamera->getCameraPosition().x << ", " << activeCamera->getCameraPosition().y << ", " << activeCamera->getCameraPosition().z << ")\n"
		          << "\t==== Pitch: " << activeCamera->getPitch() << " | Yaw: " << activeCamera->getYaw() << "\n";
		return activeCamera;
	}

	[[nodiscard]] T *getAcviveCamera() const { return activeCamera; }

  private:
	std::vector<T *> cameras;
	T *activeCamera{};
	int cameraIndex;
};
} // namespace Camera

#endif // PROYECTOFINAL_CGEIHC_CAMERACOLLECTION_H
