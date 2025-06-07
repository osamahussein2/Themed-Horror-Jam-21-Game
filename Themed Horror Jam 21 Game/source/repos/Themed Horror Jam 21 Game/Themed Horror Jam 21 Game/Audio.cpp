#include "Audio.h"

Audio::Audio() : sound(soundBuffer)
{
}

Audio::~Audio()
{
}

void Audio::InitializeAudio(const char* filePath_, bool looping_)
{
	soundBuffer.loadFromFile(filePath_);

	sound = sf::Sound(soundBuffer);
	sound.setBuffer(soundBuffer);

	sound.setLooping(looping_);
}

void Audio::PlayAudio()
{
	sound.play();
}

void Audio::StopAudio()
{
	sound.stop();
}

int Audio::GetVolume()
{
	return static_cast<int>(sound.getVolume());
}

void Audio::SetVolume(int volume_)
{
	sound.setVolume(static_cast<int>(volume_));
}
