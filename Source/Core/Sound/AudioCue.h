#ifndef __AudioCue_H__
#define __AudioCue_H__

const int AUDIO_DEFAULT_CHANNEL = -2;//A value of -2 so that nothing is played, since SDL uses a value
//of -1 when checking for ALL channels

enum AudioCueType
{
    AudioCueType_PlaySingleSound,
    AudioCueType_PlayInOrder,
    AudioCueType_PlayRandomly,
    AudioCueType_PlayAllAudio
};

enum AudioStatus 
{
	AudioStatus_Finished,
	AudioStatus_Playing,
	AudioStatus_Paused,
	AudioStatus_FadingIn,
	AudioStatus_FadingOut
};

enum PlayingStyle
{
	PlayingStyle_Single,
	PlayingStyle_Looping,
	PlayingStyle_FadeIn,
	PlayingStyle_FadeOut,
	PlayingStyle_Timed,
	PlayingStyle_TimedFadeIn,
	PlayingStyle_TimedFadeOut
};

enum AudioGroups
{
	AudioGroups_Music = 0,
	AudioGroups_Dialogues = 1,
	AudioGroups_Effects = 2

};

//IN SDL MIXER A CHANNEL OF -1 IS TO AFFECT ALL CHANNELS
class AudioCue
{
public:
private:
	int m_channel;
    
	void PlaySoundAtIndex(PlayingStyle playingType= PlayingStyle_Single,int indexAudio=0, int numLoops = -1, int fadeTime = 0, int timeLimit = 0);

protected:
    std::vector<Mix_Chunk*> m_audiosFiles;
   
	AudioCueType m_audioCueType;
	AudioGroups m_audioGroup;
	int m_audioIndex;
	
	int m_volume;
	bool m_isAudioFinished;

public:

    AudioCue(Mix_Chunk* audioChunk, AudioCueType cueType = AudioCueType_PlaySingleSound,AudioGroups audioGroup= AudioGroups_Effects);
    virtual ~AudioCue();
    void AddAudio(Mix_Chunk* audio);
    void PlayAudio(PlayingStyle playingType= PlayingStyle_Single, int audioFileToPlay = -1,int numLoops=-1, int fadeTime=0, int timeLimit =0);
    void SetCueType(AudioCueType aType){ m_audioCueType; }
    AudioCueType GetCueType(){ return m_audioCueType; }
    
	//Volume must be a value between 0 and 128.SDL Mixer ensures it is within this values.
	void SetVolume(int aVolume);
	int GetVolume(int aVolume);

	void PauseAudio();
	void UnPauseAudio();
	void StopAudio();

	AudioStatus GetAudioStatus();
};

#endif