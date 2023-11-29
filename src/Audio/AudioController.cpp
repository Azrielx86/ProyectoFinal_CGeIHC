//
// Created by edgar on 11/3/2023.
//

#include "AudioController.h"

namespace Audio
{
/**
 * Por ahora me estoy guiando por esto: https://ffainelli.github.io/openal-example/
 * @return Estado de inicialización.
 */
bool AudioController::Init()
{
	return true;
}

void AudioController::AddSource(int audioSource, Audio::AudioSource *source)
{
	sources[audioSource] = source;
}

void AudioController::PlaySource(int id)
{
	sources[id]->Play();
}

void AudioController::UpdateListenerPosition(glm::vec3 position)
{
	alListener3f(AL_POSITION, position.x, position.y, position.z);
}

void AudioController::UpdateListenerOrientation(glm::vec3 orientation, glm::vec3 up)
{
	ALfloat value[] = {orientation.x, orientation.y, orientation.z, up.x, up.y, up.z};
	alListenerfv(AL_ORIENTATION, value);
}

} // namespace Audio