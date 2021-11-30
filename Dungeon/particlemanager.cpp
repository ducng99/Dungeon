#include "particlemanager.h"
#include "particle.h"
#include "sprite.h"
#include "backbuffer.h"

ParticleManager::ParticleManager()
{
}

ParticleManager::~ParticleManager()
{
}

Particle * ParticleManager::CreateParticle(BackBuffer* backBuffer, ParticleTypes type)
{
	Particle* particle = new Particle();
	
	switch (type)
	{
	case ParticleTypes::HIT:
		for (int i = 0; i < 4; ++i)
		{
			particle->AddSprite(backBuffer->CreateSprite("assets\\imgs\\hit.png", 32 * i, 0, 32, 32));
		}
		break;
	case ParticleTypes::EXPLOSION:
		for (int i = 0; i < 7; ++i)
		{
			particle->AddSprite(backBuffer->CreateSprite("assets\\imgs\\explosion.png", 32 * i, 0, 32, 32));
		}
		break;
	default:
		break;
	}

	return particle;
}