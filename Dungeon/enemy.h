#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "entity.h"

class BackBuffer;
class Player;

enum class EnemyStates
{
	Idle_Left, Idle_Right,
	Moving_Left, Moving_Right
};

class Enemy : public Entity
{
public:
	Enemy(int prize);
	~Enemy();

	static void InitialiseSprites(BackBuffer* backBuffer);
	static void DestroySprites();

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	void SetState(EnemyStates state);

	void SetHealth(float health);
	float GetHealth();

	void SetDamage(float damage);
	float GetDamage();
	bool Attack(Player* player);

	unsigned int GetKillPrize();

	void MoveTo(Player* player);

private:
	Enemy();
	bool Initialise(Sprite* sprite);
private:
	float m_distanceTravel[2];
	
	float m_movementSpeed;

	unsigned int m_killPrize;
	float m_health;
	float m_damage;

	float m_attackTimer;
	float m_attackSpeed;

	EnemyStates m_state;
	float m_animationTimer;
	unsigned int m_animationStep;

	static Sprite* sm_pIdleSprites[];
	static Sprite* sm_pRunSprites[];
};

#endif // !__ENEMY_H__