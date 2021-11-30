#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "entity.h"
#include <vector>

class Sprite;

class Particle : public Entity
{
public:
	Particle();
	~Particle();

	void AddSprite(Sprite* sprite);

	void Process(float deltaTime);

private:
	void Initialise(Sprite* sprite);

private:
	float m_animationTimer;
	int m_animationStep;

	std::vector<Sprite*> m_pLoadedSprites;
};

#endif // !__PARTICLE_H__