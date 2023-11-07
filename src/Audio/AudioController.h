//
// Created by edgar on 11/3/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_AUDIOCONTROLLER_H
#define PROYECTOFINAL_CGEIHC_AUDIOCONTROLLER_H

#include <AL/al.h>
#include <AL/alc.h>
#include <iostream>
#include <sstream>

namespace Audio
{
class AudioController
{
  private:
	ALCdevice *device;
	ALCcontext *context;
	ALuint source;
	ALuint buffer;
	bool initialized = false;

  public:
	bool Init();
	void PlaySource(std::string source);
};

} // namespace Audio

#endif // PROYECTOFINAL_CGEIHC_AUDIOCONTROLLER_H
