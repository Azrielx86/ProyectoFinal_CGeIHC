//
// Created by edgar on 11/3/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_AUDIODEVICE_H
#define PROYECTOFINAL_CGEIHC_AUDIODEVICE_H

#include <AL/al.h>
#include <Al/alc.h>
#include <iostream>
#include <sstream>

namespace Audio
{

class AudioDevice
{
  public:
	static AudioDevice &GetInstance();
	AudioDevice(AudioDevice &) = delete;
	void operator=(const AudioDevice &) = delete;
	static void Terminate();

  private:
	static std::string getError();
	explicit AudioDevice();
	~AudioDevice();
	static AudioDevice *instance;
	ALCdevice *device;
	ALCcontext *context;
	[[maybe_unused]] bool initialized = false;
	const ALCchar *deviceName;
};

} // namespace Audio

#endif // PROYECTOFINAL_CGEIHC_AUDIODEVICE_H
