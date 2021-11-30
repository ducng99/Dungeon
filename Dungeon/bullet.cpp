#include "bullet.h"
#include "backbuffer.h"
#include "sprite.h"
#include <cmath>
#include <cassert>
#include <cmath>

Sprite* Bullet::sm_pBulletSprite;

Bullet::Bullet() : Entity()
, m_angle(0.f)
, m_flip(false)
{
	m_pSprite = sm_pBulletSprite;
	SetRadius(10.f);
}

Bullet::~Bullet()
{
	m_pSprite = 0;
}

void Bullet::InitialiseSprites(BackBuffer * backBuffer)
{
	sm_pBulletSprite = backBuffer->CreateSprite("assets\\imgs\\dungeon\\frames\\weapon_axe.png", 2);
}

void Bullet::DestroySprites()
{
	delete sm_pBulletSprite;
	sm_pBulletSprite = 0;
}

void Bullet::Process(float deltaTime)
{
	this->SetPosition(this->GetPositionX() + this->GetHorizontalVelocity() * (60.f * deltaTime), this->GetPositionY() + this->GetVerticalVelocity() * (60.f * deltaTime));

	SetAngle(GetAngle() + 500 * deltaTime);
}

void Bullet::Draw(BackBuffer & backBuffer)
{
	assert(m_pSprite);
	m_pSprite->SetX(static_cast<int>(m_x - m_pSprite->GetWidth() / 2.f));
	m_pSprite->SetY(static_cast<int>(m_y - m_pSprite->GetHeight() / 2.f));
	m_pSprite->Draw(backBuffer, (m_flip ? 360.f - m_angle : m_angle), m_flip);
}

void Bullet::SetAngle(float angle)
{
	if (angle >= 0)
	{
		this->m_angle = static_cast<float>(fmod(angle, 360));
	}
}

float Bullet::GetAngle()
{
	return this->m_angle;
}

void Bullet::SetFlip(bool flip)
{
	this->m_flip = flip;
}

bool Bullet::IsFlip()
{
	return this->m_flip;
}

void Bullet::SetupBullet(float from[2], float dest[2])
{
	m_distanceTravel[0] = from[0] - dest[0];
	m_distanceTravel[1] = from[1] - dest[1];

	if (m_distanceTravel[0] > 0)
		m_flip = true;

	this->SetPosition(from[0], from[1]);

	if (abs(m_distanceTravel[0]) > abs(m_distanceTravel[1]))
	{
		this->SetHorizontalVelocity((m_distanceTravel[0] > 0 ? -1.f : 1.f) * BULLET_SPEED);
		this->SetVerticalVelocity(abs(m_distanceTravel[1]) / abs(m_distanceTravel[0]) * BULLET_SPEED * (m_distanceTravel[1] > 0 ? -1.f : 1.f));
	}
	else
	{
		this->SetHorizontalVelocity(abs(m_distanceTravel[0]) / abs(m_distanceTravel[1]) * BULLET_SPEED * (m_distanceTravel[0] > 0 ? -1.f : 1.f));
		this->SetVerticalVelocity((m_distanceTravel[1] > 0 ? -1.f : 1.f) * BULLET_SPEED);
	}
}
