#ifndef __Sound_H__
#define __Sound_H__

class Sound
{
protected:
	FMOD::System* m_pGameSoundSystem;
	FMOD::Sound* m_pAudio;
	FMOD::Channel* m_pChannel;
	FMOD_RESULT m_result;

    float m_volume;

public:
	//If stream sound is set to false, the sound will be loaded to memory
	Sound(std:: string filepath, bool streamSound=true);
	virtual ~Sound();

	//void SetVolume(float volume);
	void PlayAudio(bool looping = false, bool startOnPause = false);
    void PauseAudio(bool aPauseStatus);
    void StopAudio();

    void SetVolume(float aVolume){ m_volume = aVolume; }
};

#endif