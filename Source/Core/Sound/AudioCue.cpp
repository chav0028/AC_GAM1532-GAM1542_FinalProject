#include "CommonHeader.h"

AudioCue::AudioCue(Mix_Chunk* audioChunk, AudioCueType cueType,AudioGroups audioGroup)
{
    if (audioChunk != nullptr)
    {
        m_audiosFiles.push_back(audioChunk);
    }

	m_audioGroup= audioGroup;
    m_audioCueType = cueType;
    m_audioIndex = 0;
	m_channel = AUDIO_DEFAULT_CHANNEL;//Set a null channel at start up
}


AudioCue::~AudioCue()
{
}

void AudioCue::AddAudio(Mix_Chunk* audio)
{
    if (audio != nullptr)
    {
        m_audiosFiles.push_back(audio);
    }
}


//Select which audio cue will be player
void AudioCue::PlayAudio(PlayingStyle aPlayingType,int aAudioFileToPlay, int aNumLoops, int aFadeTime, int aTimeLimit)
{
    //If audio file is -1, use the cue type order
    if (aAudioFileToPlay == -1)
    {
        switch (m_audioCueType)
        {
        case AudioCueType_PlaySingleSound:
            PlaySoundAtIndex(aPlayingType, m_audioIndex,aNumLoops,aFadeTime, aTimeLimit);//Play the sound at the  current index
            break;
        case AudioCueType_PlayInOrder:
            PlaySoundAtIndex(aPlayingType,m_audioIndex,aNumLoops, aFadeTime, aTimeLimit);//Play the sound at the  current index

            //Check that the index is still within range
            if (m_audioIndex >= (signed int)m_audiosFiles.size())
            {
                m_audioIndex = 0;//Reset the index
            }
            
            break;

        case AudioCueType_PlayRandomly:
        {
            //randomNumber = min + (std::rand() % (max - min + 1))
            int randomIndex = 0 + (std::rand() % (m_audiosFiles.size() - 1 - 0 + 1));//Get a rabdin ubdex
            PlaySoundAtIndex(aPlayingType,randomIndex, aNumLoops, aFadeTime, aTimeLimit);//Play the sound

            m_audioIndex = randomIndex;
        }
            break;

        case AudioCueType_PlayAllAudio:
            //Go through all the sounds
            m_audioIndex = 0;
            while (m_audioIndex<(signed int)m_audiosFiles.size())
            {
                PlaySoundAtIndex(aPlayingType,m_audioIndex, aNumLoops, aFadeTime, aTimeLimit);//Play the sound
                m_audioIndex++;
            }
            m_audioIndex = 0;
        
            break;
        default:
            break;

        }
    }
	//Play an specific audio file
    else if (aAudioFileToPlay>0)
    {
        PlaySoundAtIndex(aPlayingType, aAudioFileToPlay);
    }


}

//Volume must be a value between 0 and 128.SDL Mixer ensures it is within this values.
void AudioCue::SetVolume(int aVolume)
{		  	//Check for valid channel
	if (m_channel >= -1)
	{
		if (m_channel != SDL_MIXER_AFFECT_ALL_CHANNELS)
		{
			//If the channel is of -1 it will set all channels
			m_volume = Mix_Volume(m_channel, aVolume);//Set the current volume
		}
	}
}

//Volume must be a value between 0 and 128.SDL Mixer ensures it is within this values.
int AudioCue::GetVolume(int aVolume)
{
	//Check for valid channel
	if (m_channel >= -1)
	{
		if (m_channel != SDL_MIXER_AFFECT_ALL_CHANNELS)
		{
			//If the channel is of -1 it will return athe average volume of all the channels
			return m_volume = Mix_Volume(m_channel, m_volume);//Return the current volume
		}
	}
	return 0;
}

void  AudioCue::PlaySoundAtIndex(PlayingStyle aPlayingType,int indexAudio, int aNumLoops, int aFadeTime, int aTimeLimit)
{
	//Find a free chanel
	m_channel = Mix_GroupAvailable(m_audioGroup);
	if (m_channel == -1)
	{
		m_channel = Mix_GroupOldest(m_audioGroup);//If all channels have been taken use the last one
	}

    SetVolume(m_volume);

	//Play the sound
    if (indexAudio >= 0 && indexAudio < (signed int)m_audiosFiles.size())
    {
		if (m_audiosFiles.at(indexAudio) != nullptr)
		{
			//All values will return a value of -1 if no channel was found, or if tehre was an error
			switch (aPlayingType)
			{
			case PlayingStyle_Single:
				m_channel = Mix_PlayChannel(m_channel, m_audiosFiles.at(indexAudio), 0);
				break;
			case PlayingStyle_Looping:
				m_channel = Mix_PlayChannel(m_channel, m_audiosFiles.at(indexAudio), aNumLoops);
				break;
			case PlayingStyle_FadeIn:
				m_channel = Mix_FadeInChannel(m_channel, m_audiosFiles.at(indexAudio), aNumLoops, aFadeTime);
				break;
			case PlayingStyle_FadeOut:
				m_channel = Mix_PlayChannel(m_channel, m_audiosFiles.at(indexAudio), aNumLoops);//Play the sound
				if (m_channel != -1)
				{
					Mix_FadeOutChannel(m_channel, aFadeTime);//The fading time
				}
				break;
			case PlayingStyle_Timed:
				m_channel = Mix_PlayChannelTimed(m_channel, m_audiosFiles.at(indexAudio), aNumLoops,aTimeLimit);
				break;
			case PlayingStyle_TimedFadeIn:
				m_channel = Mix_FadeInChannelTimed(m_channel, m_audiosFiles.at(indexAudio), aNumLoops, aFadeTime,aTimeLimit);
				break;
			case PlayingStyle_TimedFadeOut:
				m_channel = Mix_PlayChannelTimed(m_channel, m_audiosFiles.at(indexAudio),aNumLoops, aTimeLimit);//Play the sound
				if (m_channel != -1)
				{
					Mix_FadeOutChannel(m_channel, aFadeTime);//The fading time
				}
				break;
			default:
				break;
			}

			//If there was an error, reset the channel
			if (m_channel == -1)
			{
				m_channel = AUDIO_DEFAULT_CHANNEL;
			}

		}
    }
}

void AudioCue::PauseAudio()
{
	//Check for valid channel
	if (m_channel >= -1)
	{
		if (m_channel != SDL_MIXER_AFFECT_ALL_CHANNELS)
		{
			Mix_Pause(m_channel);
		}
	}
}

void AudioCue::UnPauseAudio()
{
	//Check for valid channel
	if (m_channel >= -1)
	{
		if (m_channel != SDL_MIXER_AFFECT_ALL_CHANNELS)
		{
			Mix_Resume(m_channel);
		}
	}
}

void AudioCue::StopAudio()
{
	//Check for valid channel
	if (m_channel >= -1)
	{
		if (m_channel != SDL_MIXER_AFFECT_ALL_CHANNELS)
		{
			Mix_HaltChannel(m_channel);//Stop the sound

			//Reset the channel
			m_channel = AUDIO_DEFAULT_CHANNEL;
		}
	}
}

AudioStatus AudioCue::GetAudioStatus()
{
	//Check for valid channel
	if (m_channel >= -1)
	{

		//If it is not a valid channel
		if (m_channel == -1)
		{
			return AudioStatus_Finished;
		}
		else
		{
			//Order matters

			//Check fading in status
			switch (Mix_FadingChannel(m_channel))
			{
				//Channel is not fading
			case MIX_NO_FADING:
				//Don't do anything
				break;
				//Channel is fading out
			case MIX_FADING_OUT:
				return AudioStatus_FadingOut;
				break;
			case MIX_FADING_IN:
				return AudioStatus_FadingIn;
				break;
			default:
				break;
			}

			//Check if channel is paused
			int pausedStatus = Mix_Paused(m_channel);
			//Returns 0 if channel is not paused, 1 if the channel is paused
			if (pausedStatus == 1)
			{
				return AudioStatus_Paused;
			}

			//Check if channel is playing
			int playingStatus = Mix_Playing(m_channel);
			//Returns 0 if channel is not playing, 1 if the channel is playing
			if (playingStatus == 1)
			{
				return AudioStatus_Playing;
			}
		}
	}

	return AudioStatus_Finished;
}
