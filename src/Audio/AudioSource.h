//
// Created by edgar on 11/28/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_AUDIOSOURCE_H
#define PROYECTOFINAL_CGEIHC_AUDIOSOURCE_H

#include <AL/alut.h>
#include <boost/format.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <string>

namespace Audio
{
class AudioSource
{
  public:
	explicit AudioSource(const std::string &file, bool repeat = false);
	~AudioSource();
	void Init();
	void Update(const glm::vec3 &position);
	void Play();
	void Stop();

  private:
	std::string filePath;
	bool repeat = false;
	bool playing;
	ALuint sourceId;
	ALuint buffer;
	ALint source_state;
	ALfloat referenceDistance = 1.0f;
	ALfloat maxDistance = 10.0f;
	ALfloat rolloffFactor = 1.0f;
};
} // namespace Audio
#endif // PROYECTOFINAL_CGEIHC_AUDIOSOURCE_H
