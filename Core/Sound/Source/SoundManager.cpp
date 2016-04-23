#include "SoundManager.h"
#include "Wave.h"

SoundManager::SoundManager()
{
	volume_ = 1.00;
}


SoundManager::~SoundManager()
{
	m_sounds.clear();
}

void SoundManager::Initialize()
{
	initializeSound();
	isInitialized_ = true;
}

bool SoundManager::AddSound(std::string fName, std::string fPath)
{
	if (isInitialized_ == true)
	{
		m_sounds[fName];
		if (m_sounds[fName].second.load(fPath.c_str()) == true)
		{
			m_sounds[fName].first = nullptr;
			m_sound::g_engine->CreateSourceVoice(&m_sounds[fName].first, m_sounds[fName].second.wf());
			m_sounds[fName].first->SubmitSourceBuffer(m_sounds[fName].second.xaBuffer());
		}
		else
		{
			return false;
		}
		return true;
	}
	return false;
}

void SoundManager::PlaySound(std::string name)
{
	if (isInitialized_ == true)
	{
		if (m_sounds.count(name))
		{
			m_sounds[name].first->Start();
		}
	}
}

void SoundManager::PlaySoundR(std::string name)
{
	if (isInitialized_ == true)
	{
		if (m_sounds.count(name))
		{
			m_sounds[name].first->Stop();
			m_sounds[name].first->FlushSourceBuffers();
			m_sounds[name].first->SubmitSourceBuffer(m_sounds[name].second.xaBuffer());
			m_sounds[name].first->Start();
		}
	}
}

void SoundManager::StopSound(std::string name)
{
	if (isInitialized_ == true)
	{
		if (m_sounds.count(name))
		{
			m_sounds[name].first->Stop();
		}
	}
}

void SoundManager::ResetSound(std::string name)
{
	if (isInitialized_ == true)
	{
		if (m_sounds.count(name))
		{
			m_sounds[name].first->Stop();
			m_sounds[name].first->FlushSourceBuffers();
			m_sounds[name].first->SubmitSourceBuffer(m_sounds[name].second.xaBuffer());
		}
	}
	return;
}

void SoundManager::SetInfinite(std::string name)
{
	if (m_sounds.count(name))
	{
		m_sounds[name].second.LoopInfinite();
		m_sounds[name].first->FlushSourceBuffers();
		m_sounds[name].first->SubmitSourceBuffer(m_sounds[name].second.xaBuffer());
	}
}

void SoundManager::SetVolume(double volume)
{
	for (auto& iter : m_sounds)
	{
		iter.second.first->SetVolume(volume);
	}
	volume_ = volume;
}

double SoundManager::GetVolume()
{
	return volume_;
}
