#ifndef AUDIO_H
#define AUDIO_H

#include <SFML/Audio.hpp>

class Audio
{
public:
	Audio();
	~Audio();

	void InitializeAudio(const char* filePath_, bool looping_ = false);

	void PlayAudio();
	void StopAudio();

	int GetVolume();
	void SetVolume(int volume_);

	sf::Sound& GetAudio();

private:
	sf::SoundBuffer soundBuffer;
	sf::Sound sound;
};

#endif