// COMP710 GP 2D Framework 2020

// This include:
#include "entity.h"

// Local includes:
#include "sprite.h"
#include "backbuffer.h"

// Library includes:
#include <cassert>
#include <cstdlib>
#include <cstring>

Entity::Entity()
: m_pSprite(0)
, m_x(0.0f)
, m_y(0.0f)
, m_radius(0)
, m_velocityX(0.0f)
, m_velocityY(0.0f)
, m_dead(false)
{

}

Entity::~Entity()
{
	m_pSprite = 0;
}

bool
Entity::Initialise(Sprite* sprite)
{
	assert(sprite);
	m_pSprite = sprite;

	return (true);
}

void 
Entity::BoundaryDetect(int windowWidth, int windowHeight)
{
	if (this->GetPositionX() > windowWidth - 32)
	{
		this->SetPositionX(static_cast<float>(windowWidth - 32));
	}
	else if (this->GetPositionX() < 32)
	{
		this->SetPositionX(32);
	}

	if (this->GetPositionY() > windowHeight - 64)
	{
		this->SetPositionY(static_cast<float>(windowHeight - 64));
	}
	else if (this->GetPositionY() < 64)
	{
		this->SetPositionY(64);
	}
}

void 
Entity::Draw(BackBuffer& backBuffer)
{
	assert(m_pSprite);
	m_pSprite->SetX(static_cast<int>(m_x));
	m_pSprite->SetY(static_cast<int>(m_y));
	m_pSprite->Draw(backBuffer);
}

bool
Entity::IsCollidingWith(Entity& e)
{
	float delta_x = this->GetPositionX() - e.GetPositionX();
	float delta_y = this->GetPositionY() - e.GetPositionY();

	if (sqrtf(delta_x * delta_x + delta_y * delta_y) <= this->GetRadius() + e.GetRadius())
	{
		return true;
	}

	return (false);
}

void 
Entity::SetDead(bool dead)
{
	m_dead = dead;
}

bool Entity::IsDead() const
{
	return this->m_dead;
}

void
Entity::SetPosition(float x, float y)
{
	m_x = x;
	m_y = y;
}

void
Entity::SetPositionX(float x)
{
	m_x = x;
}

void
Entity::SetPositionY(float y)
{
	m_y = y;
}

float 
Entity::GetPositionX() const
{
	return (m_x);
}

float 
Entity::GetPositionY() const
{
	return (m_y);
}

void Entity::SetRadius(float rad)
{
	if (rad > 0.f)
	{
		this->m_radius = rad;
	}
}

float Entity::GetRadius()
{
	return this->m_radius;
}

float 
Entity::GetHorizontalVelocity() const
{
	return (m_velocityX);
}

float 
Entity::GetVerticalVelocity() const
{
	return (m_velocityY);
}

void 
Entity::SetHorizontalVelocity(float x)
{
	m_velocityX = x;
}

void 
Entity::SetVerticalVelocity(float y)
{
	m_velocityY = y;
}
