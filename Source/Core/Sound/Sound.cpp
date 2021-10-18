#include "CommonHeader.h"
#include "Sound.h"


Sound::Sound(std::string aFilepath, bool aStreamSound):
	m_pGameSoundSystem(nullptr),
	m_pAudio(nullptr),
	m_pChannel(nullptr),
    m_volume(1.0f)
{
	//Get the FMOD sound system from the game
	m_pGameSoundSystem = g_pGame->GetFMODSoundSystem();

	if (m_pGameSoundSystem != nullptr)
	{
		//If sound is being streamed
		if (aStreamSound == true)
		{
			m_result=m_pGameSoundSystem->createStream(aFilepath.c_str(), FMOD_DEFAULT, 0, &m_pAudio);
		}
		else//If sound loaded to memory
		{
			m_result=m_pGameSoundSystem->createSound(aFilepath.c_str(), FMOD_DEFAULT, 0, &m_pAudio);
		}

		SoundSystem::ErrorCheckFMOD(m_result);
	}

	 
}

Sound::~Sound()
{
	//Sound system deleted in game

	m_pAudio->release();//Release and delete audio
	m_pAudio = nullptr;
	//SAFE_DELETE(m_pAudio);


	//SAFE_DELETE(m_pChannel);
}

void Sound::PlayAudio(bool aLooping, bool aStartOnPause)
{
    //Load the channel
    //if (m_pChannel == nullptr)
    //{
        //Start the channel on pause so that it can be modified
        m_result = m_pGameSoundSystem->playSound(FMOD_CHANNEL_FREE, m_pAudio, true, &m_pChannel);
        SoundSystem::ErrorCheckFMOD(m_result);

        if (m_result != FMOD_ERR_INVALID_HANDLE)//if a valid channel was found
        {
            //If audio will loop indefinitely
            if (aLooping == true)
            {
                if (m_pChannel != nullptr)
                {
                    //Set it to be looping
                    m_result = m_pChannel->setMode(FMOD_LOOP_NORMAL);

                    m_pChannel->setLoopCount(-1);

                    SoundSystem::ErrorCheckFMOD(m_result);
                }
            }
        }
    //}
          
        //Play the sound
        if (m_pChannel != nullptr && m_pGameSoundSystem != nullptr && m_pAudio != nullptr)
        {
            // Set sound properties
            m_pChannel->setVolume(m_volume); 
            
            m_result = m_pChannel->setPaused(aStartOnPause); //Unpause, or keep paused the sound now that its properties have been set
            SoundSystem::ErrorCheckFMOD(m_result);
        }

}

void Sound::StopAudio()
{
    if (m_pChannel != nullptr)//Stop the sound
    {
        m_result = m_pChannel->stop();
        SoundSystem::ErrorCheckFMOD(m_result);
    }
}

void Sound::PauseAudio(bool aPauseStatus)
{
    if (m_pChannel != nullptr)
    {
        m_result = m_pChannel->setPaused(aPauseStatus);
        SoundSystem::ErrorCheckFMOD(m_result);


    }   
}