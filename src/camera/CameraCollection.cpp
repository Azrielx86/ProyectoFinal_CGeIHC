//
// Created by edgar on 11/2/2023.
//

#include "CameraCollection.h"

namespace Camera
{
Camera *CameraCollection::getAcviveCamera() const { return acviveCamera; }
CameraCollection &CameraCollection::addCamera(const Camera &camera)
{
	cameras.push_back(camera);
	if (acviveCamera == nullptr)
		acviveCamera = &cameras.front();
	return *this;
}
} // namespace Camera