#ifndef __SOUND_H__
#define __SOUND_H__

#include <fmod.hpp>
#include <map>
#include <string>

enum class Sounds
{
	HIT, EXPLOSION, DIE
};

class SoundManager
{
public:
	static SoundManager& GetInstance();
	static void DestroyInstance();
	~SoundManager();

	void Initialise();

	void LoadSound(Sounds sound, const char* pcFilename);
	void PlaySound(Sounds sound, float volume = 0.3f);
private:
	SoundManager(const SoundManager& sound);
	SoundManager& operator=(const SoundManager& sound);

	SoundManager();

private:
	static SoundManager* sm_pInstance;
	FMOD::Channel* m_pChannel;
	FMOD::System* m_pFMODSystem;

	std::map<Sounds, FMOD::Sound*> m_pLoadedSounds;
};

#endif // !__SOUND_H__
