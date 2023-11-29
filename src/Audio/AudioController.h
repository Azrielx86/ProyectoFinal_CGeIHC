//
// Created by edgar on 11/3/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_AUDIOCONTROLLER_H
#define PROYECTOFINAL_CGEIHC_AUDIOCONTROLLER_H

#include "AudioSource.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>

namespace Audio
{
class AudioController
{
  public:
	bool Init();
	void AddSource(int audioSource, Audio::AudioSource *source);
	void PlaySource(int id);
	static void UpdateListenerPosition(glm::vec3 position);
	static void UpdateListenerOrientation(glm::vec3 orientation, glm::vec3 up);

  private:
	std::unordered_map<int, Audio::AudioSource *> sources;
};

} // namespace Audio

#endif // PROYECTOFINAL_CGEIHC_AUDIOCONTROLLER_H
