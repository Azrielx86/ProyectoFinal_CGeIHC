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
	initialized = true;
	return initialized;
}

void AudioController::PlaySource(std::string source)
{
	ALsizei size, freq;
	ALenum format;
	ALvoid *data;
	ALboolean loop = AL_FALSE;
	
	
}
} // namespace Audio