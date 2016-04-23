#pragma once
#include <windows.h>
#include <xaudio2.h>
#include <fstream>

namespace m_sound
{
	extern IXAudio2* g_engine;
	extern IXAudio2SourceVoice* g_source;
	extern IXAudio2MasteringVoice* g_master;
}

int initializeSound();

class Wave
{
private:
	WAVEFORMATEX m_wf;
	XAUDIO2_BUFFER m_xa;
	BYTE* m_waveData;
public:
	Wave(const char* szFile = NULL);
	Wave(const Wave& c);
	~Wave();

	void LoopInfinite();

	const XAUDIO2_BUFFER* xaBuffer() const;
	const WAVEFORMATEX* wf() const;
	bool load(const char* szFile);
};