#include "CommonHeader.h"

//Code based on tutorial : https://katyscode.wordpress.com/2012/10/05/cutting-your-teeth-on-fmod-part-1-build-environment-initialization-and-playing-sounds/


SoundSystem::SoundSystem():
	m_pSystem(nullptr)
{
	//Create Fmod interface object pointer
	m_result = FMOD::System_Create(&m_pSystem);
	ErrorCheckFMOD(m_result);//Error checking

	//Check version dll and library matches
	m_result = m_pSystem->getVersion(&m_version);//Get dll version
	ErrorCheckFMOD(m_result);
		  
	if (m_version < FMOD_VERSION)
	{
		OutputDebugString("FMOD library version doesn't match DLL version. Probably using old version of FMOD");
	}

	//Get the number of sound cards in the system
	m_result = m_pSystem->getNumDrivers(&m_numberDrivers);
	ErrorCheckFMOD(m_result);			 

	//If there are no sound cards, disable sound
	if (m_numberDrivers == 0)
	{
		
		//Set output type
		m_result = m_pSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);

		//Display error message
		OutputDebugString("No sound card driver found /n");

		ErrorCheckFMOD(m_result);
	}
	else//If there is a soundcard
	{
		// Get the  details of the default (active) sound card
		m_result = m_pSystem->getDriverCaps(0, &m_soundCardCapabilities, 0, &m_speakerMode);
		ErrorCheckFMOD(m_result);

		// Set the speaker mode
		m_result = m_pSystem->setSpeakerMode(m_speakerMode);
		ErrorCheckFMOD(m_result);
	}

	//Check if hardware acceleration is disabled
	if (m_soundCardCapabilities & FMOD_CAPS_HARDWARE_EMULATED)
	{
		//Increase the buffer size to prevent stuttering
		m_result = m_pSystem->setDSPBufferSize(1024, 10);//Parameters:Number of samples in buffer, number of buffers
		ErrorCheckFMOD(m_result);
	}


	//The following is a kludge for SigmaTel sound drivers.We first get the name of the first enumerated sound card driver by specifying zero as the first parameter to getDriverInfo(the 4th parameter receives the device GUID which we ignore here).If it contains the string ‘SigmaTel’, the output format is changed to PCM floating point, and all the other format settings are left as the sound card’s current settings.
	//	// Get name of driver
	//	result = system->getDriverInfo(0, name, 256, 0);
	//FMODErrorCheck(result);
	//// SigmaTel sound devices crackle for some reason if the format is PCM 16-bit.
	//// PCM floating point output seems to solve it.
	//if (strstr(name, "SigmaTel"))
	//{
	//	result = system->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR);
	//	FMODErrorCheck(result);
	//}

	//Initialize FMOD
	m_result = m_pSystem->init(100, FMOD_INIT_NORMAL, 0);

	// If the speaker mode isn't supported by the sound card
	if (m_result == FMOD_ERR_OUTPUT_CREATEBUFFER)//FMOD_ERR_OUTPUT_CREATEBUFFER is value given in inti
	{
		m_result = m_pSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO); //Set , by default, speaker mode to stereo
		ErrorCheckFMOD(m_result);

		m_result = m_pSystem->init(100, FMOD_INIT_NORMAL, 0);//Initialize FMOD again
	}


	ErrorCheckFMOD(m_result);


}

SoundSystem::~SoundSystem()
{
	if (m_pSystem != nullptr)
	{
		m_pSystem->release();//Release FMOD interface and deletes memory
		m_pSystem = nullptr;
	}
}


void SoundSystem::ErrorCheckFMOD(FMOD_RESULT aResult)
{
	if(aResult !=FMOD_OK) //If there is an error in the result
	{
		//Display error messages
		OutputDebugString("FMOD ERROR:");
		OutputDebugString(FMOD_ErrorString(aResult));
		OutputDebugString("/n");
	}

}