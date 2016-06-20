#pragma once
#include <string>
#include "Timer.h"

class SoundPlayer
{
public:
	SoundPlayer();
	~SoundPlayer();

	void Update();
	void Tick();
	void SetSound(std::string sound);
	void SetTimer(double time);
	void StopSound();

private:
	Timer soundStop_;
	std::string soundName;
	bool soundPlaying_;
};

