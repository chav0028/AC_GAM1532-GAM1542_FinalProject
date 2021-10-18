#ifndef __SoundSystem_H__
#define __SoundSystem_H__

#include "fmod.h"

class SoundSystem
{
private:
	FMOD::System *m_pSystem;
	FMOD_RESULT m_result;
	unsigned int m_version;
	int m_numberDrivers;
	FMOD_SPEAKERMODE m_speakerMode;
	FMOD_CAPS m_soundCardCapabilities;
	char m_name[256];

public:
	SoundSystem();
	~SoundSystem();
	
	
	static void ErrorCheckFMOD(FMOD_RESULT result);

	FMOD::System* GetSoundSystem() { return m_pSystem; }
};

#endif