#ifndef __PARTICLEMANAGER_H__
#define __PARTICLEMANAGER_H__

#include <map>

class BackBuffer;
class Particle;

enum class ParticleTypes
{
	HIT, EXPLOSION
};

class ParticleManager
{
public:
	ParticleManager();
	~ParticleManager();

	Particle* CreateParticle(BackBuffer* backBuffer, ParticleTypes type);

private:
	ParticleManager(const ParticleManager& particleManager);
	ParticleManager& operator=(const ParticleManager& particleManager);

};

#endif // !__PARTICLEMANAGER_H__