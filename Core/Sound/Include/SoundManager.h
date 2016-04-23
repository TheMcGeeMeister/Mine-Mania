#pragma once
#include <string>
#include <map>
#include <xaudio2.h>
#include <Wave.h>

class SoundManager
{
public:
	SoundManager();
	~SoundManager();
	void Initialize();

	bool AddSound(std::string fName, std::string fPath);

	void PlaySound(std::string name);
	void PlaySoundR(std::string name); // Reset before playing
	void StopSound(std::string name);
	void ResetSound(std::string name);
	void SetInfinite(std::string name);
	void SetVolume(double volume);

	double GetVolume();
private:
	std::map<std::string, std::pair<IXAudio2SourceVoice*, Wave> > m_sounds;

	bool isInitialized_;

	double volume_;
};

