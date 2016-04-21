#pragma once
#include <Timer.h>
#include <string>

class SoundPlayer
{
public:
	SoundPlayer();
	~SoundPlayer();

	void Update();
	void SetSound(std::string sound);
	void SetTimer(double time);

private:
	Timer soundStop_;
	std::string soundName;
	bool soundPlaying_;
};

