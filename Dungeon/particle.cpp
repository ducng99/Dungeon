#include "particle.h"
#include "sprite.h"
#include <cassert>

Particle::Particle() : Entity()
, m_animationTimer(0)
, m_animationStep(0)
{

}

Particle::~Particle()
{
	for (Sprite* sprite : m_pLoadedSprites)
	{
		delete sprite;
		sprite = 0;
	}

	m_pLoadedSprites.clear();
	m_pLoadedSprites.shrink_to_fit();
}

void Particle::AddSprite(Sprite * sprite)
{
	m_pLoadedSprites.push_back(sprite);

	if (m_pSprite == 0)
	{
		m_pSprite = sprite;
	}
}

void Particle::Process(float deltaTime)
{
	if (m_animationStep == m_pLoadedSprites.size() - 1)
	{
		this->SetDead(true);
	}
	else
	{
		m_animationTimer += deltaTime;

		if (m_animationTimer >= 0.2f)
		{
			m_animationTimer = 0;

			m_pSprite = m_pLoadedSprites[++m_animationStep % m_pLoadedSprites.size()];
		}
	}
}