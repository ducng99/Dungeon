#include "enemy.h"
#include "player.h"
#include "backbuffer.h"
#include "sprite.h"
#include "sound.h"
#include <cstdlib>
#include <string>
#include <cassert>

Sprite* Enemy::sm_pIdleSprites[4];
Sprite* Enemy::sm_pRunSprites[4];

Enemy::Enemy(int prize) : Entity()
, m_health(100)
, m_damage(5)
, m_killPrize(100)
, m_movementSpeed(0.5f)
, m_state(EnemyStates::Idle_Right)
, m_animationTimer(0)
, m_animationStep(0)
, m_attackTimer(0)
, m_attackSpeed(1.f)
{
	SetRadius(25.f);
	this->m_killPrize = prize;
	m_movementSpeed = (rand() % (8 - 3) + 3) / 10.f;

	assert(sm_pIdleSprites[0]);
	m_pSprite = sm_pIdleSprites[0];
}

Enemy::~Enemy()
{
	m_pSprite = 0;
}

void Enemy::InitialiseSprites(BackBuffer * backBuffer)
{
	for (int i = 0; i < 4; ++i)
	{
		std::string path;

		path = "assets\\imgs\\dungeon\\frames\\big_demon_idle_anim_f" + std::to_string(i) + ".png";
		sm_pIdleSprites[i] = backBuffer->CreateSprite(path.c_str(), 2);

		path = "assets\\imgs\\dungeon\\frames\\big_demon_run_anim_f" + std::to_string(i) + ".png";
		sm_pRunSprites[i] = backBuffer->CreateSprite(path.c_str(), 2);
	}
}

void Enemy::DestroySprites()
{
	for (int i = 0; i < 4; ++i)
	{
		delete sm_pIdleSprites[i];
		sm_pIdleSprites[i] = 0;
		delete sm_pRunSprites[i];
		sm_pRunSprites[i] = 0;
	}
}

void Enemy::Process(float deltaTime)
{
	if (this->GetHealth() <= 0)
		this->SetDead(true);

	m_attackTimer += deltaTime;
	m_animationTimer += deltaTime;

	if (m_animationTimer >= 0.25f)
	{
		m_animationTimer = 0;

		if (this->GetHorizontalVelocity() > 0)
		{
			SetState(EnemyStates::Moving_Right);
		}
		else if (this->GetHorizontalVelocity() < 0)
		{
			SetState(EnemyStates::Moving_Left);
		}
		else if (this->GetVerticalVelocity() > 0 || this->GetVerticalVelocity() < 0)
		{
			if (static_cast<int>(m_state) % 2 == 0)
			{
				SetState(EnemyStates::Moving_Left);
			}
			else
			{
				SetState(EnemyStates::Moving_Right);
			}
		}
		else
		{
			if (static_cast<int>(m_state) % 2 == 0)
			{
				SetState(EnemyStates::Idle_Left);
			}
			else
			{
				SetState(EnemyStates::Idle_Right);
			}
		}

		switch (m_state)
		{
		case EnemyStates::Idle_Left:
		case EnemyStates::Idle_Right:
			m_pSprite = sm_pIdleSprites[m_animationStep++ % 4];
			break;
		case EnemyStates::Moving_Left:
		case EnemyStates::Moving_Right:
			m_pSprite = sm_pRunSprites[m_animationStep++ % 4];
			break;
		default:
			break;
		}
	}

	this->SetPositionX(this->GetPositionX() + this->GetHorizontalVelocity() * deltaTime * 60.f);
	this->SetPositionY(this->GetPositionY() + this->GetVerticalVelocity() * deltaTime * 60.f);
}

void Enemy::Draw(BackBuffer & backBuffer)
{
	assert(m_pSprite);
	m_pSprite->SetX(static_cast<int>(m_x - m_pSprite->GetWidth() / 2.f));
	m_pSprite->SetY(static_cast<int>(m_y - m_pSprite->GetHeight() / 2.f));
	m_pSprite->Draw(backBuffer, 0, static_cast<int>(m_state) % 2 == 0);
}

void Enemy::SetState(EnemyStates state)
{
	if (m_state != state)
	{
		m_animationStep = 0;
		m_state = state;
	}
}

void Enemy::SetHealth(float health)
{
	if (health >= 0)
	{
		this->m_health = health;
	}
	else
	{
		this->m_health = 0;
	}
}

float Enemy::GetHealth()
{
	return this->m_health;
}

void Enemy::SetDamage(float damage)
{
	if (damage >= 0.f)
	{
		this->m_damage = damage;
	}
}

float Enemy::GetDamage()
{
	return this->m_damage;
}

bool Enemy::Attack(Player * player)
{
	if (m_attackTimer > m_attackSpeed && this->IsCollidingWith(*player) && player->GetHealth() > 0)
	{
		player->SetHealth(player->GetHealth() - this->GetDamage());

		m_attackTimer = 0;

		return true;
	}

	return false;
}

unsigned int Enemy::GetKillPrize()
{
	return this->m_killPrize;
}

void Enemy::MoveTo(Player* player)
{
	m_distanceTravel[0] = this->GetPositionX() - player->GetPositionX();
	m_distanceTravel[1] = this->GetPositionY() - player->GetPositionY();

	if (this->IsCollidingWith(*player))
	{
		this->SetHorizontalVelocity(0);
		this->SetVerticalVelocity(0);
	}
	else
	{
		if (abs(m_distanceTravel[0]) > abs(m_distanceTravel[1]))
		{
			this->SetHorizontalVelocity((m_distanceTravel[0] > 0 ? -1.f : 1.f) * m_movementSpeed);
			this->SetVerticalVelocity(abs(m_distanceTravel[1]) / abs(m_distanceTravel[0]) * m_movementSpeed * (m_distanceTravel[1] > 0 ? -1.f : 1.f));
		}
		else
		{
			this->SetHorizontalVelocity(abs(m_distanceTravel[0]) / abs(m_distanceTravel[1]) * m_movementSpeed * (m_distanceTravel[0] > 0 ? -1.f : 1.f));
			this->SetVerticalVelocity((m_distanceTravel[1] > 0 ? -1.f : 1.f) * m_movementSpeed);
		}
	}
}
