#include "..\include\SoundPlayer.h"
#include "SoundManager.h"

namespace game
{
	extern SoundManager m_sounds;
}


SoundPlayer::SoundPlayer()
{
	soundPlaying_ = false;
}


SoundPlayer::~SoundPlayer()
{
}

void SoundPlayer::Update()
{
	if (soundStop_.Update() == true)
	{
		if (soundPlaying_ == true)
		{
			game::m_sounds.StopSound(soundName);
			soundPlaying_ = false;
		}
	}
}

void SoundPlayer::SetSound(std::string sound)
{
	soundName = sound;
}

void SoundPlayer::SetTimer(double time)
{
	soundStop_.StartNewTimer(time);
	if (soundPlaying_ == false)
	{
		game::m_sounds.PlaySoundR(soundName);
		soundPlaying_ = true;
	}
}
