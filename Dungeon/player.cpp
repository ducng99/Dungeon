#include "player.h"
#include "backbuffer.h"
#include "sprite.h"
#include <string>
#include <cassert>

Sprite* Player::sm_pIdleSprites[4];
Sprite* Player::sm_pHitSprites[1];
Sprite* Player::sm_pRunSprites[4];

Player::Player() : Entity()
, m_state(PlayerStates::Idle_Right)
, m_animationTimer(0)
, m_animationStep(0)
, m_attackTimer(0)
, m_attackSpeed(0.7f)
, m_health(100)
, m_maxHealth(100)
, m_damage(20)
, m_killCount(0)
, m_money(100)
, MOVEMENT_SPEED(0.2f)
, m_maxMoveSpeed(1.0f)
{
	assert(sm_pIdleSprites[0]);

	m_pSprite = sm_pIdleSprites[0];
	SetRadius(20.f);
}

Player::~Player()
{
	m_pSprite = 0;
}

void Player::InitializeSprites(BackBuffer * backBuffer)
{
	for (int i = 0; i < 4; ++i)
	{
		std::string path;

		path = "assets\\imgs\\dungeon\\frames\\knight_m_idle_anim_f" + std::to_string(i) + ".png";
		sm_pIdleSprites[i] = backBuffer->CreateSprite(path.c_str(), 2.f);

		path = "assets\\imgs\\dungeon\\frames\\knight_m_run_anim_f" + std::to_string(i) + ".png";
		sm_pRunSprites[i] = backBuffer->CreateSprite(path.c_str(), 2.f);
	}

	sm_pHitSprites[0] = backBuffer->CreateSprite("assets\\imgs\\dungeon\\frames\\knight_m_hit_anim_f0.png", 2.f);
}

void Player::DestroySprites()
{
	for (int i = 0; i < 4; ++i)
	{
		delete sm_pIdleSprites[i];
		sm_pIdleSprites[i] = 0;

		delete sm_pRunSprites[i];
		sm_pRunSprites[i] = 0;
	}

	delete sm_pHitSprites[0];
	sm_pHitSprites[0] = 0;
}

void Player::Process(float deltaTime)
{
	if (this->GetHealth() == 0)
		this->SetDead(true);

	m_attackTimer += deltaTime;
	m_animationTimer += deltaTime;

	if (m_animationTimer >= 0.25f)
	{
		m_animationTimer = 0;

		if (this->GetHorizontalVelocity() > 0)
		{
			SetState(PlayerStates::Moving_Right);
		}
		else if (this->GetHorizontalVelocity() < 0)
		{
			SetState(PlayerStates::Moving_Left);
		}
		else if (this->GetVerticalVelocity() > 0 || this->GetVerticalVelocity() < 0)
		{
			if (static_cast<int>(m_state) % 2 == 0)
			{
				SetState(PlayerStates::Moving_Left);
			}
			else
			{
				SetState(PlayerStates::Moving_Right);
			}
		}
		else
		{
			if (static_cast<int>(m_state) % 2 == 0)
			{
				SetState(PlayerStates::Idle_Left);
			}
			else
			{
				SetState(PlayerStates::Idle_Right);
			}
		}

		switch (m_state)
		{
		case PlayerStates::Idle_Left:
		case PlayerStates::Idle_Right:
			m_pSprite = sm_pIdleSprites[m_animationStep++ % 4];
			break;
		case PlayerStates::Moving_Left:
		case PlayerStates::Moving_Right:
			m_pSprite = sm_pRunSprites[m_animationStep++ % 4];
			break;
		case PlayerStates::Hit_Left:
		case PlayerStates::Hit_Right:
			m_pSprite = sm_pHitSprites[0];
			break;
		default:
			break;
		}
	}
	
	this->SetPosition(this->GetPositionX() + this->GetHorizontalVelocity() * (60.f * deltaTime), this->GetPositionY() + this->GetVerticalVelocity() * (60.f * deltaTime));
}

void Player::Draw(BackBuffer & backBuffer)
{
	assert(m_pSprite);
	m_pSprite->SetX(static_cast<int>(m_x - m_pSprite->GetWidth() / 2.f));
	m_pSprite->SetY(static_cast<int>(m_y - m_pSprite->GetHeight() / 2.f));
	m_pSprite->Draw(backBuffer, 0, static_cast<int>(m_state) % 2 == 0);
}

void Player::SetState(PlayerStates state)
{
	if (m_state != state)
	{
		m_animationStep = 0;
		m_state = state;
	}
}

void Player::MoveLeft()
{
	if (this->GetHorizontalVelocity() > -m_maxMoveSpeed)
		this->SetHorizontalVelocity(this->GetHorizontalVelocity() - MOVEMENT_SPEED);

	m_state = PlayerStates::Moving_Left;
}

void Player::MoveRight()
{
	if (this->GetHorizontalVelocity() < m_maxMoveSpeed)
		this->SetHorizontalVelocity(this->GetHorizontalVelocity() + MOVEMENT_SPEED);

	m_state = PlayerStates::Moving_Right;
}

void Player::MoveDown()
{
	if (this->GetVerticalVelocity() < m_maxMoveSpeed)
		this->SetVerticalVelocity(this->GetVerticalVelocity() + MOVEMENT_SPEED);

	if (static_cast<int>(m_state) % 2 == 0)
	{
		m_state = PlayerStates::Moving_Left;
	}
	else
	{
		m_state = PlayerStates::Moving_Right;
	}
}

void Player::MoveUp()
{
	if (this->GetVerticalVelocity() > -m_maxMoveSpeed)
		this->SetVerticalVelocity(this->GetVerticalVelocity() - MOVEMENT_SPEED);

	if (static_cast<int>(m_state) % 2 == 0)
	{
		m_state = PlayerStates::Moving_Left;
	}
	else
	{
		m_state = PlayerStates::Moving_Right;
	}
}

void Player::SetHealth(float health)
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

float Player::GetHealth()
{
	return this->m_health;
}

void Player::SetMaxHealth(float health)
{
	if (health > 0)
	{
		this->m_maxHealth = health;
	}
}

float Player::GetMaxHealth()
{
	return this->m_maxHealth;
}

float Player::GetHealthRatio()
{
	return this->m_health / this->m_maxHealth;
}

void Player::SetDamage(float damage)
{
	if (damage >= 0.f)
	{
		this->m_damage = damage;
	}
}

float Player::GetDamage()
{
	return m_damage;
}

void Player::SetAttackSpeed(float speed)
{
	if (speed > 0)
	{
		this->m_attackSpeed = speed;
	}
}

float Player::GetAttackSpeed()
{
	return this->m_attackSpeed;
}

void Player::SetMovementSpeed(float speed)
{
	if (speed > 0)
	{
		this->m_maxMoveSpeed = speed;
	}
}

float Player::GetMovementSpeed()
{
	return this->m_maxMoveSpeed;
}

bool Player::IsAttackable()
{
	if (m_attackTimer >= m_attackSpeed)
	{
		m_attackTimer = 0;
		return true;
	}

	return false;
}

void Player::DecreaseHorizontalVelocity()
{
	if (this->GetHorizontalVelocity() > 0)
	{
		if (this->GetHorizontalVelocity() < MOVEMENT_SPEED)
		{
			this->SetHorizontalVelocity(0);
		}
		else
		{
			this->SetHorizontalVelocity(this->GetHorizontalVelocity() - MOVEMENT_SPEED);
		}
	}
	else if (this->GetHorizontalVelocity() < 0)
	{
		if (this->GetHorizontalVelocity() > -MOVEMENT_SPEED)
		{
			this->SetHorizontalVelocity(0);
		}
		else
		{
			this->SetHorizontalVelocity(this->GetHorizontalVelocity() + MOVEMENT_SPEED);
		}
	}
}

void Player::DecreaseVerticalVelocity()
{
	if (this->GetVerticalVelocity() > 0)
	{
		if (this->GetVerticalVelocity() < MOVEMENT_SPEED)
		{
			this->SetVerticalVelocity(0);
		}
		else
		{
			this->SetVerticalVelocity(this->GetVerticalVelocity() - MOVEMENT_SPEED);
		}
	}
	else if (this->GetVerticalVelocity() < 0)
	{
		if (this->GetVerticalVelocity() > -MOVEMENT_SPEED)
		{
			this->SetVerticalVelocity(0);
		}
		else
		{
			this->SetVerticalVelocity(this->GetVerticalVelocity() + MOVEMENT_SPEED);
		}
	}
}

unsigned int Player::GetMoney()
{
	return this->m_money;
}

void Player::SetMoney(unsigned int money)
{
	this->m_money = money;
}

unsigned int Player::GetKillCount()
{
	return this->m_killCount;
}

void Player::SetKillCount(int count)
{
	this->m_killCount = count;
}
