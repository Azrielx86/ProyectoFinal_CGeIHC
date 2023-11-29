//
// Created by edgar on 11/28/2023.
//

#include "AudioSource.h"

namespace Audio
{
AudioSource::AudioSource(const std::string &file, bool repeat)
{
	this->filePath = file;
	this->repeat = repeat;
}

void AudioSource::Init()
{
	ALCenum error;
	alGenSources(1, &sourceId);

	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		std::cerr << boost::format("[%s] Error generating source: %s\n") % typeid(AudioSource).name() % alutGetErrorString(error);
	}

	buffer = alutCreateBufferFromFile(this->filePath.c_str());
	error = alutGetError();
	if (error != AL_NO_ERROR)
	{
		std::cerr << boost::format("[%s] Error generating buffer: %s\n") % typeid(AudioSource).name() % alutGetErrorString(error);
	}

	alSourcei(sourceId, AL_BUFFER, buffer); // NOLINT(*-narrowing-conversions)
	alSourcei(sourceId, AL_LOOPING, repeat ? AL_TRUE : AL_FALSE);
	alSourcef(sourceId, AL_PITCH, 1.0f);
	alSourcef(sourceId, AL_GAIN, 1.0f);
	alSource3f(sourceId, AL_POSITION, 0.0f, 0.0f, 0.0f);
	
	alSourcef(sourceId, AL_REFERENCE_DISTANCE, referenceDistance);
	alSourcef(sourceId, AL_MAX_DISTANCE, maxDistance);
	alSourcef(sourceId, AL_ROLLOFF_FACTOR, rolloffFactor);
}

void AudioSource::Update(const glm::vec3 &position)
{
	ALCenum error = alGetError();
	alGetSourcei(sourceId, AL_SOURCE_STATE, &source_state);
	alSource3f(sourceId, AL_POSITION, position.x, position.y, position.z);
	if (error != AL_NO_ERROR)
		std::cerr << boost::format("[%s] Error on update: %s\n") % typeid(AudioSource).name() % alutGetErrorString(error);
	alSource3f(sourceId, AL_POSITION, 0.0f, 0.0f, 0.0f);
}

void AudioSource::Play()
{
	if (playing) return;
	alSourcePlay(sourceId);
	ALCenum error = alGetError();
	if (error != AL_NO_ERROR)
		std::cerr << boost::format("[%s] Error playing source: %s\n") % typeid(AudioSource).name() % alutGetErrorString(error);
	playing = true;
}

void AudioSource::Stop()
{
	alSourceStop(sourceId);
	ALCenum error = alGetError();
	if (error != AL_NO_ERROR)
		std::cerr << boost::format("[%s] Error stopping source: %s\n") % typeid(AudioSource).name() % alutGetErrorString(error);
	playing = false;
}

AudioSource::~AudioSource()
{
	alDeleteSources(1, &sourceId);
	alDeleteBuffers(1, &buffer);
}

} // namespace Audio
