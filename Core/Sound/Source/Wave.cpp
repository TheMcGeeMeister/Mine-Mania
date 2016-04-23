#include "Wave.h"

namespace m_sound
{
	IXAudio2* g_engine;
	IXAudio2SourceVoice* g_source;
	IXAudio2MasteringVoice* g_master;
}

int initializeSound()
{
	using namespace m_sound;
	//must call this for COM
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	//create the engine
	if (FAILED(XAudio2Create(&g_engine)))
	{
		CoUninitialize();
		return -1;
	}

	//create the mastering voice
	if (FAILED(g_engine->CreateMasteringVoice(&g_master)))
	{
		g_engine->Release();
		CoUninitialize();
		return -2;
	}

	//helper class to load wave files; trust me, this makes it MUCH easier
	//Wave buffer;

	//load a wave file
	/*if (!buffer.load("sfx.wav"))
	{
	g_engine->Release();
	CoUninitialize();
	return -3;
	}

	//create the source voice, based on loaded wave format
	if (FAILED(g_engine->CreateSourceVoice(&g_source, buffer.wf())))
	{
	g_engine->Release();
	CoUninitialize();
	return -4;
	}*/

}

Wave::Wave(const char* szFile) : m_waveData(NULL)
{
	ZeroMemory(&m_wf, sizeof(m_wf));
	ZeroMemory(&m_xa, sizeof(m_xa));

	load(szFile);
}

Wave::Wave(const Wave& c) : m_waveData(NULL)
{
	m_wf = c.m_wf;
	m_xa = c.m_xa;
	if (c.m_waveData)
	{
		m_waveData = new BYTE[m_xa.AudioBytes];
		memcpy(m_waveData, c.m_waveData, m_xa.AudioBytes);
		m_xa.pAudioData = m_waveData;
	}
}

Wave::~Wave()
{
	if (m_waveData)
		delete[] m_waveData;
	m_waveData = NULL;
}

void Wave::LoopInfinite()
{
	m_xa.LoopCount = XAUDIO2_LOOP_INFINITE;
}

const XAUDIO2_BUFFER* Wave::xaBuffer() const 
{ 
	return &m_xa;
}

const WAVEFORMATEX* Wave::wf() const 
{ 
	return &m_wf; 
}

bool Wave::load(const char* szFile)
{
	if (szFile == NULL)
		return false;

	std::ifstream inFile(szFile, std::ios::binary | std::ios::in);
	if (inFile.bad())
		return false;

	DWORD dwChunkId = 0, dwFileSize = 0, dwChunkSize = 0, dwExtra = 0;

	//look for 'RIFF' chunk identifier
	inFile.seekg(0, std::ios::beg);
	inFile.read(reinterpret_cast<char*>(&dwChunkId), sizeof(dwChunkId));
	if (dwChunkId != 'FFIR')
	{
		inFile.close();
		return false;
	}
	inFile.seekg(4, std::ios::beg); //get file size
	inFile.read(reinterpret_cast<char*>(&dwFileSize), sizeof(dwFileSize));
	if (dwFileSize <= 16)
	{
		inFile.close();
		return false;
	}
	inFile.seekg(8, std::ios::beg); //get file format
	inFile.read(reinterpret_cast<char*>(&dwExtra), sizeof(dwExtra));
	if (dwExtra != 'EVAW')
	{
		inFile.close();
		return false;
	}

	//look for 'fmt ' chunk id
	bool bFilledFormat = false;
	for (unsigned int i = 12; i < dwFileSize; )
	{
		inFile.seekg(i, std::ios::beg);
		inFile.read(reinterpret_cast<char*>(&dwChunkId), sizeof(dwChunkId));
		inFile.seekg(i + 4, std::ios::beg);
		inFile.read(reinterpret_cast<char*>(&dwChunkSize), sizeof(dwChunkSize));
		if (dwChunkId == ' tmf')
		{
			inFile.seekg(i + 8, std::ios::beg);
			inFile.read(reinterpret_cast<char*>(&m_wf), sizeof(m_wf));
			bFilledFormat = true;
			break;
		}
		dwChunkSize += 8; //add offsets of the chunk id, and chunk size data entries
		dwChunkSize += 1;
		dwChunkSize &= 0xfffffffe; //guarantees WORD padding alignment
		i += dwChunkSize;
	}
	if (!bFilledFormat)
	{
		inFile.close();
		return false;
	}

	//look for 'data' chunk id
	bool bFilledData = false;
	for (unsigned int i = 12; i < dwFileSize; )
	{
		inFile.seekg(i, std::ios::beg);
		inFile.read(reinterpret_cast<char*>(&dwChunkId), sizeof(dwChunkId));
		inFile.seekg(i + 4, std::ios::beg);
		inFile.read(reinterpret_cast<char*>(&dwChunkSize), sizeof(dwChunkSize));
		if (dwChunkId == 'atad')
		{
			m_waveData = new BYTE[dwChunkSize];
			inFile.seekg(i + 8, std::ios::beg);
			inFile.read(reinterpret_cast<char*>(m_waveData), dwChunkSize);
			m_xa.AudioBytes = dwChunkSize;
			m_xa.pAudioData = m_waveData;
			m_xa.PlayBegin = 0;
			m_xa.PlayLength = 0;
			bFilledData = true;
			break;
		}
		dwChunkSize += 8; //add offsets of the chunk id, and chunk size data entries
		dwChunkSize += 1;
		dwChunkSize &= 0xfffffffe; //guarantees WORD padding alignment
		i += dwChunkSize;
	}
	if (!bFilledData)
	{
		inFile.close();
		return false;
	}


	inFile.close();
	return true;
}