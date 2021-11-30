#ifndef __BULLET_H__
#define __BULLET_H__

#include "entity.h"

class BackBuffer;

class Bullet : public Entity
{
public:
	Bullet();
	~Bullet();

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	void SetAngle(float angle);
	float GetAngle();

	void SetFlip(bool flip);
	bool IsFlip();

	void SetupBullet(float from[2], float dest[2]);

	static void InitialiseSprites(BackBuffer* backBuffer);
	static void DestroySprites();
private:
	bool Initialise(Sprite* sprite);
private:
	float m_distanceTravel[2];

	float m_angle;
	bool m_flip;

	const float BULLET_SPEED = 3.f;
	static Sprite* sm_pBulletSprite;
};

#endif // !__BULLET_H__