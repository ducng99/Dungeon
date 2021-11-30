#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"

class BackBuffer;

enum class PlayerStates
{
	Idle_Left, Idle_Right,
	Moving_Left, Moving_Right,
	Hit_Left, Hit_Right
};

class Player : public Entity
{
public:
	Player();
	~Player();

	static void InitializeSprites(BackBuffer* backBuffer);
	static void DestroySprites();

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	void SetState(PlayerStates state);

	void MoveLeft();
	void MoveRight();
	void MoveDown();
	void MoveUp();

	void SetHealth(float health);
	float GetHealth();
	void SetMaxHealth(float health);
	float GetMaxHealth();
	float GetHealthRatio();

	void SetDamage(float damage);
	float GetDamage();
	void SetAttackSpeed(float speed);
	float GetAttackSpeed();
	void SetMovementSpeed(float speed);
	float GetMovementSpeed();
	bool IsAttackable();

	void DecreaseHorizontalVelocity();
	void DecreaseVerticalVelocity();

	unsigned int GetMoney();
	void SetMoney(unsigned int money);
	unsigned int GetKillCount();
	void SetKillCount(int count);
private:
	bool Initialise(Sprite* sprite);

private:
	const float MOVEMENT_SPEED;
	float m_maxMoveSpeed;
	float m_damage;

	unsigned int m_killCount;
	unsigned int m_money;
	float m_health;
	float m_maxHealth;

	float m_animationTimer;
	unsigned int m_animationStep;
	PlayerStates m_state;

	float m_attackSpeed;
	float m_attackTimer;

	static Sprite* sm_pIdleSprites[];
	static Sprite* sm_pHitSprites[];
	static Sprite* sm_pRunSprites[];
};

#endif // !__PLAYER_H__