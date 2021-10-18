#ifndef __Audio_H__
#define __Audio_H__

class AudioCue
{
protected:
    std::Vector<Mix_chunk>

public:
    //If stream sound is set to false, the sound will be loaded to memory
    Sound(std::string filepath, bool streamSound = true);
    virtual ~Sound();

    //void SetVolume(float volume);
    void PlayAudio(bool looping = false, bool startOnPause = false);
    void PauseAudio(bool aPauseStatus);
    void StopAudio();

    void SetVolume(float aVolume){ m_volume = aVolume; }
};

#endif