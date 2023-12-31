//
// Created by edgar on 11/3/2023.
//

#include "AudioDevice.h"

namespace Audio
{

AudioDevice *AudioDevice::instance = nullptr;

AudioDevice::AudioDevice()
{
	device = alcOpenDevice(nullptr);
	if (!device)
	{
		std::cerr << getError();
		throw std::exception();
	}

	context = alcCreateContext(device, nullptr);
	if (!alcMakeContextCurrent(context))
	{
		std::cerr << getError();
		throw std::exception();
	}

	deviceName = nullptr;
	deviceName = alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);

	std::cout << "[ " << typeid(AudioDevice).name()
	          << " ] Audio device created succesfully!. "
	          << "Using: " << deviceName << "\n";
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
}

AudioDevice &AudioDevice::GetInstance()
{
	if (instance == nullptr)
		instance = new AudioDevice();
	return *instance;
}

std::string AudioDevice::getError()
{
	std::stringstream stream;
	ALCenum error;
	error = alGetError();
	stream << "Error opening the audio device: " << error << "\n";
	return stream.str();
}

AudioDevice::~AudioDevice()
{
	alcDestroyContext(context);
	alcCloseDevice(device);
}

void AudioDevice::Terminate()
{
	alutExit();
	delete instance;
	std::cout << "[ " << typeid(AudioDevice).name()
	          << " ] Audio device closed.\n";
}

void AudioDevice::InitAlut(int *argc, char **argv)
{
	alutInit(argc, argv);
}
} // namespace Audio