#include "sound.h"
#include "logmanager.h"
#include <fmod.hpp>
#include <cassert>

using namespace FMOD;
SoundManager* SoundManager::sm_pInstance = 0;

SoundManager& SoundManager::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new SoundManager();
	}

	assert(sm_pInstance);
	return *sm_pInstance;
}

void SoundManager::DestroyInstance()
{
	delete sm_pInstance;
	sm_pInstance = 0;
}

SoundManager::~SoundManager()
{
	std::map<Sounds, Sound*>::iterator it = m_pLoadedSounds.begin();

	while (it != m_pLoadedSounds.end())
	{
		it->second->release();
		it->second = 0;

		it++;
	}

	m_pChannel = 0;

	m_pFMODSystem->close();
	m_pFMODSystem->release();
	m_pFMODSystem = 0;
}

SoundManager::SoundManager()
: m_pFMODSystem(0)
, m_pChannel(0)
{

}

void SoundManager::Initialise()
{
	System_Create(&m_pFMODSystem);
	assert(m_pFMODSystem);
	m_pFMODSystem->init(1, FMOD_INIT_NORMAL, 0);
}

void SoundManager::LoadSound(Sounds sound, const char* pcFilename)
{
	Sound* pSound = 0;

	if (m_pLoadedSounds.find(sound) == m_pLoadedSounds.end())
	{
		m_pFMODSystem->createSound(pcFilename, FMOD_DEFAULT, 0, &pSound);

		if (pSound == 0)
		{
			LogManager::GetInstance().Log("Load sound failed!");
		}

		m_pLoadedSounds[sound] = pSound;
	}
}

void SoundManager::PlaySound(Sounds sound, float volume)
{
	m_pFMODSystem->playSound(m_pLoadedSounds[sound], 0, false, &m_pChannel);
	m_pChannel->setVolume(volume);
}
